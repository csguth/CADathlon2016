#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>
#include <cmath>
#include <array>

static constexpr unsigned IMAGE_WIDTH=3;
static constexpr unsigned IMAGE_HEIGHT=3;
static constexpr unsigned IMAGE_SIZE=IMAGE_WIDTH*IMAGE_HEIGHT;
static constexpr unsigned MAXIMUM_GRAY_LEVEL=255;
using pixel_type = unsigned;
using frame_storage_type = std::vector<pixel_type>;

#define MAP(x,y) x*IMAGE_WIDTH+y

class Frame{
    private:
        frame_storage_type red_component, green_component, blue_component;

    public:        
        Frame() : red_component(IMAGE_SIZE), green_component(IMAGE_SIZE), blue_component(IMAGE_SIZE)
        {
        }
        
        const frame_storage_type & get_red() const {return red_component;}
        const frame_storage_type & get_green() const {return green_component;}
        const frame_storage_type & get_blue() const {return blue_component;}

        pixel_type red(unsigned x, unsigned y) const {return red_component.at(MAP(x,y));}
        pixel_type green(unsigned x, unsigned y) const {return green_component.at(MAP(x,y));}
        pixel_type blue(unsigned x, unsigned y) const {return blue_component.at(MAP(x,y));}

        void red(unsigned x, unsigned y, pixel_type value){red_component.at(MAP(x,y)) = value;}
        void green(unsigned x, unsigned y, pixel_type value){green_component.at(MAP(x,y)) = value;}
        void blue(unsigned x, unsigned y, pixel_type value){blue_component.at(MAP(x,y)) = value;}
};


double compute_mean(const Frame & frame)
{
    double mean = std::accumulate(frame.get_red().begin(), frame.get_red().end(), 0.0);
    mean = std::accumulate(frame.get_green().begin(), frame.get_green().end(), mean);
    mean = std::accumulate(frame.get_blue().begin(), frame.get_blue().end(), mean);
    mean = mean/(frame.get_red().size()*3);
    return mean;
}

double compute_interval_mean(const Frame & frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double mean = 0.0;
    int den = 0;
    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            mean += frame.red(i,j) + frame.green(i,j) + frame.blue(i,j);
    }
    mean = mean/(den*3);
    return mean;
}

double compute_variance(const Frame & frame)
{
    double mean = compute_mean(frame);
    double variance = 0.0;
    for(auto value : frame.get_red())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_green())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_blue())
        variance += ((value - mean)*(value - mean));

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    variance = variance/(frame.get_red().size()*3 - 1);
    return variance;
}

double compute_interval_variance(const Frame & frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double mean = compute_interval_mean(frame, x_interval, y_interval);
    double variance = 0.0;
    int den = 0;

    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            variance += (frame.red(i,j) - mean)*(frame.red(i,j) - mean)
                     +  (frame.green(i,j) - mean)*(frame.green(i,j) - mean)
                     +  (frame.blue(i,j) - mean)*(frame.blue(i,j) - mean);

    }

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    variance = variance/(den*3 - 1);
    return variance;
}

double compute_covariance(const Frame & new_frame, const Frame & original_frame)
{
    double new_frame_mean = compute_mean(new_frame); 
    double original_frame_mean = compute_mean(original_frame); 

    double covariance = 0.0;
    auto it_original = original_frame.get_red().begin();
    for(auto it_new = new_frame.get_red().begin(); it_new != new_frame.get_red().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_green().begin();
    for(auto it_new = new_frame.get_green().begin(); it_new != new_frame.get_green().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_blue().begin();
    for(auto it_new = new_frame.get_blue().begin(); it_new != new_frame.get_blue().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    covariance = covariance/(new_frame.get_red().size()*3 - 1);
    return covariance;
}

double compute_interval_covariance(const Frame & new_frame, const Frame & original_frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double new_frame_mean = compute_interval_mean(new_frame, x_interval, y_interval); 
    double original_frame_mean = compute_interval_mean(original_frame, x_interval, y_interval); 

    double covariance = 0.0;
    int den = 0;

    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            covariance += (new_frame.red(i,j) - new_frame_mean)*(original_frame.red(i,j) - original_frame_mean)
                     +  (new_frame.green(i,j) - new_frame_mean)*(original_frame.green(i,j) - original_frame_mean)
                     +  (new_frame.blue(i,j) - new_frame_mean)*(original_frame.blue(i,j) - original_frame_mean);

    }

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    covariance = covariance/(den*3 - 1);
    return covariance;
}

double compute_ssim(const Frame & new_frame, const Frame & original_frame)
{
    const double C1 = 2.55;
    const double C2 = 7.56;
    double ssim = 0.0;

    double new_frame_mean = compute_mean(new_frame);
    double new_frame_variance = compute_variance(new_frame);

    double original_frame_mean = compute_mean(original_frame);
    double original_frame_variance = compute_variance(original_frame);

    double covariance = compute_covariance(new_frame, original_frame);

    ssim = ((2*new_frame_mean*original_frame_mean + C1)*(2*covariance + C2))/
                 ((std::pow(new_frame_mean,2) + std::pow(original_frame_mean,2) + C1)*(new_frame_variance + original_frame_variance + C2)); 

    return ssim;
}

double compute_interval_ssim(const Frame & new_frame, const Frame & original_frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    const double C1 = 2.55;
    const double C2 = 7.56;
    double ssim = 0.0;

    double new_frame_mean = compute_interval_mean(new_frame, x_interval, y_interval);
    double new_frame_variance = compute_interval_variance(new_frame, x_interval, y_interval);

    double original_frame_mean = compute_interval_mean(original_frame, x_interval, y_interval);
    double original_frame_variance = compute_interval_variance(original_frame, x_interval, y_interval);

    double covariance = compute_interval_covariance(new_frame, original_frame, x_interval, y_interval);

    ssim = ((2*new_frame_mean*original_frame_mean + C1)*(2*covariance + C2))/
                 ((std::pow(new_frame_mean,2) + std::pow(original_frame_mean,2) + C1)*(new_frame_variance + original_frame_variance + C2)); 

    return ssim;
}

void print_frame(const Frame & frame)
{
    std::cout << "**Red Component**" << std::endl;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j)
            std::cout << frame.red(i,j) << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "**Green Component**" << std::endl;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j)
            std::cout << frame.green(i,j) << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "**Blue Component**" << std::endl;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j)
            std::cout << frame.blue(i,j) << "\t";
        std::cout << std::endl;
    }
    std::cout << std::endl;


}

pixel_type total_sacrificed_luminance(const Frame & frame, pixel_type threshold)
{
    pixel_type total_sacrificed_luminance = std::accumulate(frame.get_red().begin(), frame.get_red().end(), 0.0, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : 0; });
    total_sacrificed_luminance = std::accumulate(frame.get_green().begin(), frame.get_green().end(), total_sacrificed_luminance, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : 0; });
    total_sacrificed_luminance = std::accumulate(frame.get_blue().begin(), frame.get_blue().end(), total_sacrificed_luminance, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : 0; });
    return total_sacrificed_luminance;
}

double sacrificed_luminance_ratio(const Frame & frame, pixel_type threshold)
{
    return total_sacrificed_luminance(frame, threshold)/total_sacrificed_luminance(frame, 0);
}

int main() {
    std::cout << "OLED Display DVS" << std::endl;

    
    static constexpr std::array<double, 9> new_red_component {
              { 255, 254, 253, 252, 251, 250, 249, 248, 247 }
    };

    static constexpr std::array<double, 9> new_green_component {
              { 207, 206, 205, 204, 203, 202, 201, 200, 199 }
    };

    static constexpr std::array<double, 9> new_blue_component {
              { 150, 151, 152, 153, 154, 155, 156, 157, 157 }
    };

    Frame frame;

    unsigned index=0;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j, ++index)
            frame.red(i,j, new_red_component[index]);
    }

    index=0;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j, ++index)
            frame.green(i,j, new_green_component[index]);
    }

    index=0;
    for(unsigned i = 0; i < IMAGE_WIDTH; ++i)
    {
        for(unsigned j = 0; j < IMAGE_HEIGHT; ++j, ++index)
            frame.blue(i,j, new_blue_component[index]);
    }

    print_frame(frame);

    auto ssim = compute_ssim(frame, frame);
    std::cout << "SSIM: " << ssim << std::endl;
    assert(ssim == 1);

    auto interval_ssim = compute_interval_ssim(frame, frame, {0,1}, {0,1});
    std::cout << "Interval SSIM: " << interval_ssim << std::endl;
    assert(interval_ssim == 1);

    assert(total_sacrificed_luminance(frame, 255) == 0);
    assert(total_sacrificed_luminance(frame, 0) == 5471);

    std::cout << "Done!" << std::endl;
    return 0;
}
