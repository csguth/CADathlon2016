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
        std::cout << "Elmore::~Elmore()" << std::endl;
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


static constexpr std::array<double, 15> Rs {
    {0.63096,
    0.79433,
    1.00000,
    1.25892,
    1.58489,
    1.99526,
    2.51189,
    3.16228,
    3.98107,
    5.01187,
    6.30957,
    7.94328,
    10.00000,
    12.58925,
    15.84893}
};

static constexpr std::array<double, 15> log10R {
    {-0.2,
    -0.1,
    0.0,
    0.1,
    0.2,
    0.3,
    0.4,
    0.5,
    0.6,
    0.7,
    0.8,
    0.9,
    1.0,
    1.1,
    1.2 }
};

static constexpr std::array<double, 15> thetas {
    { 0.48837,
    0.76029,
    1.00000,
    1.22371,
    1.43757,
    1.64467,
    1.84678,
    2.04507,
    2.24031,
    2.43305,
    2.62371,
    2.81262,
    3.00000,
    3.18607,
    3.37098 }
};

double GAMMA(double value) {
    if(value > 2.0) {
        return (value-1.0)*GAMMA(value-1.0);
    }
    static constexpr std::array<double, 11> LUT {
      {1.0,
       0.95135,
       0.91817,
        0.89747,
        0.88726,
        0.88623,
        0.89352,
        0.90864,
        0.93138,
        0.96176,
        1.0}
    };
   value -= 1.0;
   value *= 10.0;
   const uint32_t index = static_cast<uint32_t>(std::round(value));
   return LUT[index];
}

double D2M(double m1, double m2) {
    return std::pow(m1, 2.0)*std::log(2.0) / std::sqrt(m2);
}

double f(const double kR, const double x) {
    auto den = GAMMA(1.0+x);
    auto denSquared = den*den;
    return (GAMMA(1.0+(x+x))/(denSquared+denSquared))-kR;
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
    std::function<double(double)> fx = std::bind(&f, r, std::placeholders::_1);
    return bisection(0.48837, 3.37098, fx);
}


int main(int argc, char* argv[]) {
    RCTree tree(8);
    
    tree.cap(tree.fromId(0), 0.0);
    tree.cap(tree.fromId(1), 0.5e-12);
    tree.cap(tree.fromId(2), 1e-12);
    tree.cap(tree.fromId(3), 1e-12);
    tree.cap(tree.fromId(4), 1e-12);
    tree.cap(tree.fromId(5), 1e-12);
    tree.cap(tree.fromId(6), 1e-12);
    tree.cap(tree.fromId(7), 1e-12);

    tree.connect(tree.fromId(0), tree.fromId(1), 80.0);
    tree.connect(tree.fromId(1), tree.fromId(2), 60.0);
    tree.connect(tree.fromId(2), tree.fromId(3), 60.0);
    tree.connect(tree.fromId(3), tree.fromId(4), 60.0);
    tree.connect(tree.fromId(4), tree.fromId(5), 60.0);
    tree.connect(tree.fromId(1), tree.fromId(6), 60.0);
    tree.connect(tree.fromId(6), tree.fromId(7), 60.0);

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

    for(auto it = tree.G().nodes_begin(); it != tree.G().nodes_end(); ++it) {
        auto node = *it;
        printf("node %d\n", node.id());
        printf("   ED = %lf ns\n", -1.0e9 * el.moment<1>(node));
        printf("  D2M = %lf ns\n", 1.0e9 * D2M(el.moment<1>(node), el.moment<2>(node)));
        const double kR = el.moment<2>(node)/std::pow(el.moment<1>(node), 2.0);
        printf("    r = %lf\n", kR);
        printf("log(r)= %lf\n", std::log10(kR));
        const double theta = THETA(kR);
        printf("theta = %lf\n", theta);
        printf("gamma = %lf %lf\n", GAMMA(1.0+theta), std::tgamma(1.0+theta));
        const double beta = -el.moment<1>(node)/GAMMA(1.0+theta);
        printf(" beta = %lf\n", beta);
        printf("  WbD = %lf ps\n", 1e12 * beta*std::pow(std::log(2.0), theta));
    }
    std::cout << std::endl;
   
    printf("using vars %zu %zu %zu\n", Rs.size(), log10R.size(), thetas.size());

    return 0;
}
