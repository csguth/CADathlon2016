#include <iostream>
#include <GTL/graph.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <queue>
#include <cstdio>
#include <array>
#include <cmath>
#include <cassert>
#include <functional>
#include <numeric>

template <class T>
struct range {
  T begin_;
  T end_;
  T begin() { return begin_; }
  T end() { return end_; }
};

template <class T>
range<T> makeRange(T begin, T end) {
   return range<T>{begin, end};
}

double linearInterpolation(const double y1, const double y0, const double x1, const double x0, const double x) {
    const double kA = (y1-y0)/(x1-x0);
    const double kB = y0;
    const double kX = x - x0;
    return kA*kX + kB;
}

class RCTree final {
public:
   RCTree(uint32_t nodes) :
       nodes_(nodes),
       order_(nodes),
       capacitance_(G_),
       resistance_(G_)
   {
     G_.make_undirected();
     nodes_.resize(0);
     order_.resize(0);
     for(uint32_t i = 0; i < nodes; ++i) {
        nodes_.push_back(G_.new_node());
     } 
   }

   ~RCTree() {

   }\

   node fromId(uint32_t id) {
       return nodes_[id];
   }

   void connect(const node& u, const node& v, double resistance) {
      auto edge = G_.new_edge(u, v);
      resistance_[edge] = resistance;
   }
   void cap(const node & u, double capacitance) {
     capacitance_[u] = capacitance;
   }

   void build(const node& source) {
     node_map<bool> touched(G_);
     auto nodes = makeRange(G_.nodes_begin(), G_.nodes_end());
     for(const node& n : nodes) {
         touched[n] = false;
     }
     std::queue<node> Q;
     {
         Q.push(source);
         touched[source] = true;
     }
     pred_.reset(new node_map< std::tuple<node, edge> >(G_));
     while(!Q.empty()) {
        node &curr = Q.front();
        order_.push_back(curr);
        for(auto it = curr.adj_edges_begin(); it != curr.adj_edges_end(); ++it) {
            edge e = *it;
            node opposite = curr.opposite(e);
            if(!touched[opposite]) {
                touched[opposite] = true;
                Q.push(opposite);
                (*pred_)[opposite] = std::make_tuple(curr, e);
            }
        }
        Q.pop();
     } 

    std::cout << "order: ";
     for(auto el : order_) {
         std::cout << el.id() << " ";
     }
     std::cout << std::endl;
   } 

   double cap(const node& u) const {
       return capacitance_[u]; 
   }

   const std::vector<node> & order() const {
       return order_;
   }

   std::tuple<node, edge> pred(const node& n) const {
       return (*pred_)[n];
   }

   double res(const edge& e) const {
       return resistance_[e];
   }

   graph& G() {
      return G_;
   }

private:
   graph G_;
   std::vector<node> nodes_;
   std::vector<node> order_;
   node_map<double> capacitance_;
   edge_map<double> resistance_;
   std::unique_ptr<node_map< std::tuple<node, edge> > > pred_;
};

class Elmore final {
public:
    Elmore(RCTree& tree) :
        tree_(tree),
        firstMoment_(tree.G()),
        secondMoment_(tree.G())
    {
        
    }
    ~Elmore() {

    }
    void computeLumped(node_map<double>& lumped)
    {
        for(uint32_t i = tree_.order().size() - 1; i > 0; --i) {
            auto curr = tree_.order()[i];
            auto pred = tree_.pred(curr);
            lumped[std::get<0>(pred)] += lumped[curr];
        }
    }

    void run() {
        node_map<double> lumped(tree_.G());
        auto order = tree_.order();
        for(auto node : order) {
            lumped[node] = tree_.cap(node);
        }
        computeLumped(lumped);
        compute(firstMoment_, lumped);
        for(auto node : order) {
            lumped[node] = tree_.cap(node) * firstMoment_[node];
        }
        computeLumped(lumped);
        compute(secondMoment_, lumped);
    }

    void compute(node_map<double>& moment, node_map<double> & lumped) {
        moment[tree_.order().front()] = 0.0;
        auto const kSize = tree_.order().size();
        for(uint32_t i = 1; i < kSize; ++i) {
            auto curr = tree_.order()[i];
            auto pred = tree_.pred(curr);
            moment[curr] = moment[std::get<0>(pred)] - lumped[curr] * tree_.res(std::get<1>(pred));
            std::cout << "moment " << curr.id() << " = mom pred " << moment[std::get<0>(pred)] << " - lumped " << lumped[curr] << " * res " << tree_.res(std::get<1>(pred)) << " = " <<  moment[curr] << std::endl;
        }
    }
    
    template<uint32_t N>
    double moment(const node& n) const;

private:
   RCTree& tree_;
   node_map<double> firstMoment_;
   node_map<double> secondMoment_;
};

template <>
double Elmore::moment<0>(const node&n) const {
    return 1.0;
}

template <>
double Elmore::moment<1>(const node&n) const {
    return firstMoment_[n];
}

template <>
double Elmore::moment<2>(const node&n) const {
    return secondMoment_[n];
}

double GAMMA(double value) {
#ifdef USE_STD_GAMMA
    return std::tgamma(value);
#else
    if(value > 2.0) {
        return (value-1.0)*GAMMA(value-1.0);
    }
    static constexpr std::array<double, 11> indices {
      { 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0 }
    };
    static constexpr std::array<double, 11> LUT {
      { 1.0, 0.95135, 0.91817, 0.89747, 0.88726, 0.88623, 0.89352, 0.90864, 0.93138, 0.96176, 1.0}
    };
   const uint32_t index = static_cast<uint32_t>(std::ceil((value*10.0)-10.0));
   if(index == 0)
   {
       return LUT[index];
   }
   return linearInterpolation(LUT[index], LUT[index-1], indices[index], indices[index-1], value);
#endif
}

double D2M(double m1, double m2) {
    return std::pow(m1, 2.0)*std::log(2.0) / std::sqrt(m2);
}

double f(const double kR, const double x) {
    auto den = GAMMA(1.0+x);
    auto denSquared = den*den;
    return 0.5*(GAMMA(1.0+(x+x))/(denSquared+denSquared))-kR;
}

double bisection(double a, double b, std::function<double(double)> fx) {
    const uint32_t NMAX = 50;
    uint32_t N = 1;
    const double kEpsilon = 1e-10;
    while(N <= NMAX) {
        const double c = (a+b)/2.0;
        const double fxc = fx(c);
        if(std::abs(fxc) < kEpsilon || (b-a)/2.0 < kEpsilon ) {
            return c;
        }
        ++N;
        if(std::signbit(fxc) == std::signbit(fx(a)))
        {
            a = c;
        } else {
            b = c;
        }
    }
    assert(false);
}

double THETA(double r) {
    static constexpr std::array<double, 15> log10R {
        {-0.2, -0.1, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2 }
    };
    static constexpr std::array<double, 15> thetas {
        { 0.48837, 0.76029, 1.00000, 1.22371, 1.43757, 1.64467, 1.84678, 2.04507, 2.24031, 2.43305, 2.62371, 2.81262, 3.00000, 3.18607, 3.37098 }
    };
    const double logR = std::log10(r);
    auto index = static_cast<uint32_t>(std::max(0.0, std::ceil(10.0*(logR-log10R[0]))));
    assert(index == std::distance(log10R.begin(), std::lower_bound(log10R.begin(), log10R.end(), logR)));
    if(index == 0) {
        ++index;
    }
    return linearInterpolation(thetas[index], thetas[index-1], log10R[index], log10R[index-1], logR);
}

class Weibull final {
public:
    Weibull(const Elmore & elmore, RCTree & tree) :
        elmore_(elmore),
        tree_(tree),
        delays_(tree_.G())
    {

    }

    ~Weibull() {

    }

    void run() {
        auto range = makeRange(tree_.G().nodes_begin(), tree_.G().nodes_end());
        for(auto node : range) {
            const double m1 = elmore_.moment<1>(node);
            const double m2 = elmore_.moment<2>(node);
            const double r = m2 / (m1 * m1);
            const double theta = THETA(r);
            const double lutGamma = GAMMA(1.0+theta);
            const double stdGamma = std::tgamma(1.0+theta);
            const double beta = -m1/GAMMA(1.0+theta);
            delays_[node] = beta*std::pow(std::log(2.0), theta);
            printf("- node %d\n", node.id());
            printf("     moments %lf %lf\n", m1, m2);
            printf("           r %lf\n", r);
            printf("       theta %lf\n", theta);
            printf("   LUT Gamma %lf\n", lutGamma);
            printf("   STD Gamma %lf\n", stdGamma);
            printf("        beta %lf\n", beta);
            printf("          ED %lf\n", -m1);
            printf("         D2M %lf\n", D2M(m1, m2));
            printf("         WED %lf\n", delays_[node]);
        }
    }

private:
    const Elmore & elmore_;
    RCTree & tree_;
    node_map<double> delays_;
};

int main(int argc, char* argv[]) {
    RCTree tree(8);

    tree.cap(tree.fromId(0), 0.0);
    tree.cap(tree.fromId(1), 500);
    tree.cap(tree.fromId(2), 1000);
    tree.cap(tree.fromId(3), 1000);
    tree.cap(tree.fromId(4), 1000);
    tree.cap(tree.fromId(5), 1200);
    tree.cap(tree.fromId(6), 1000);
    tree.cap(tree.fromId(7), 1200);

    tree.connect(tree.fromId(0), tree.fromId(1), 80e-3);
    tree.connect(tree.fromId(1), tree.fromId(2), 60e-3);
    tree.connect(tree.fromId(2), tree.fromId(3), 60e-3);
    tree.connect(tree.fromId(3), tree.fromId(4), 60e-3);
    tree.connect(tree.fromId(4), tree.fromId(5), 60e-3);
    tree.connect(tree.fromId(1), tree.fromId(6), 60e-3);
    tree.connect(tree.fromId(6), tree.fromId(7), 60e-3);

//    RCTree tree(7);
//    tree.cap(tree.fromId(0), 0.0);
//    tree.cap(tree.fromId(1), 10e-9);
//    tree.cap(tree.fromId(2), 13e-9);
//    tree.cap(tree.fromId(3), 11e-9);
//    tree.cap(tree.fromId(4), 12e-9);
//    tree.cap(tree.fromId(5), 17e-9);
//    tree.cap(tree.fromId(6), 14e-9);

//    tree.connect(tree.fromId(0), tree.fromId(1), 5);
//    tree.connect(tree.fromId(1), tree.fromId(2), 16);
//    tree.connect(tree.fromId(2), tree.fromId(3), 15);
//    tree.connect(tree.fromId(3), tree.fromId(4), 3);
//    tree.connect(tree.fromId(4), tree.fromId(5), 11);
//    tree.connect(tree.fromId(5), tree.fromId(6), 19);
  
    tree.build(tree.fromId(0));
    
    Elmore el(tree);
    el.run();

    Weibull weibull(el, tree);
    weibull.run();

    std::cout << "some thetas..." << std::endl;
    std::cout << "  theta(0.5) = " << THETA(0.5) << std::endl;
    std::cout << "  theta(0.63096) = " << THETA(0.63096) << std::endl;
    std::cout << "  theta(5.01187) = " << THETA(5.01187) << std::endl;
    std::cout << "  theta(10.0) = " << THETA(10.0) << std::endl;
    std::cout << "  theta(15.84893) = " << THETA(15.84893) << std::endl;

    return 0;
}
