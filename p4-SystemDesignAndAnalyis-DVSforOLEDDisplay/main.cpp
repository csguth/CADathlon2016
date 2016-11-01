#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>
#include <cmath>

static constexpr unsigned IMAGE_WIDTH=3;
static constexpr unsigned IMAGE_HEIGHT=3;
static constexpr unsigned IMAGE_SIZE=IMAGE_WIDTH*IMAGE_HEIGHT;
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
        
        const frame_storage_type & get_red_component() const {return red_component;}
        const frame_storage_type & get_green_component() const {return green_component;}
        const frame_storage_type & get_blue_component() const {return blue_component;}

        pixel_type red(unsigned x, unsigned y){return red_component.at(MAP(x,y));}
        pixel_type green(unsigned x, unsigned y){return green_component.at(MAP(x,y));}
        pixel_type blue(unsigned x, unsigned y){return blue_component.at(MAP(x,y));}

        void red(unsigned x, unsigned y, pixel_type value){red_component.at(MAP(x,y)) = value;}
        void green(unsigned x, unsigned y, pixel_type value){green_component.at(MAP(x,y)) = value;}
        void blue(unsigned x, unsigned y, pixel_type value){blue_component.at(MAP(x,y)) = value;}
};


double compute_mean(const Frame & frame)
{
    double mean = std::accumulate(frame.get_red_component().begin(), frame.get_red_component().end(), 0.0);
    mean = std::accumulate(frame.get_green_component().begin(), frame.get_green_component().end(), mean);
    mean = std::accumulate(frame.get_blue_component().begin(), frame.get_blue_component().end(), mean);
    mean = mean/(frame.get_red_component().size()*3);
    return mean;
}

double compute_variance(const Frame & frame)
{
    double mean = compute_mean(frame);
    double variance = 0.0;
    for(auto value : frame.get_red_component())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_green_component())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_blue_component())
        variance += ((value - mean)*(value - mean));

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    variance = variance/(frame.get_red_component().size()*3 - 1);
    return variance;
}

double compute_covariance(const Frame & new_frame, const Frame & original_frame)
{
    double new_frame_mean = compute_mean(new_frame); 
    double original_frame_mean = compute_mean(original_frame); 

    double covariance = 0.0;
    auto it_original = original_frame.get_red_component().begin();
    for(auto it_new = new_frame.get_red_component().begin(); it_new != new_frame.get_red_component().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_green_component().begin();
    for(auto it_new = new_frame.get_green_component().begin(); it_new != new_frame.get_green_component().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_blue_component().begin();
    for(auto it_new = new_frame.get_blue_component().begin(); it_new != new_frame.get_blue_component().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    covariance = covariance/(new_frame.get_red_component().size()*3 - 1);
    return covariance;
}

std::vector<double> compute_ssim(const Frame & new_frame, const Frame & original_frame)
{
    const double C1 = 2.55;
    const double C2 = 7.56;
    std::vector<double> ssim(3, 0.0); //red, green, and blue SSIMs

    double new_frame_mean = compute_mean(new_frame);
    double new_frame_variance = compute_variance(new_frame);

    double original_frame_mean = compute_mean(original_frame);
    double original_frame_variance = compute_variance(original_frame);

    double covariance = compute_covariance(new_frame, original_frame);

    ssim.at(0) = ((2*new_frame_mean*original_frame_mean + C1)*(2*covariance + C2))/
                 ((std::pow(new_frame_mean,2) + std::pow(original_frame_mean,2) + C1)*(new_frame_variance + original_frame_variance + C2)); 

    return ssim;
}

int main() {
    std::cout << "OLED Display DVS" << std::endl;

    return 0;
}
