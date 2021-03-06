#pragma once
#include <Eigen/Dense>
#include <random>
#include <vector>
class MOO
{
public:
    enum CrowdingSpace{Input=0, Output=1};
protected:
    // if _record_all is set to true, the all the evaluated points would be saved to calculate a pareto front
    bool _record_all = false;
    size_t _np = 100;
    size_t _gen = 200;
    double _f = 0.8;
    double _cr = 0.8;
    size_t _seed = std::random_device{}();
    size_t _eval_counter = 0;
    const size_t _dim;
    const size_t _num_obj;
    const Eigen::VectorXd _lb;
    const Eigen::VectorXd _ub;

    Eigen::MatrixXd _anchor_x;
    Eigen::MatrixXd _anchor_y;
    Eigen::MatrixXd _pop_x;
    Eigen::MatrixXd _pop_y;
    Eigen::MatrixXd _elitist_x;
    Eigen::MatrixXd _elitist_y;

    std::mt19937_64 _engine;
    Eigen::VectorXi _ranks;
    Eigen::VectorXd _crowding_vol;
    CrowdingSpace   _cs;

    Eigen::MatrixXd _mutation(double f, const Eigen::MatrixXd& parent);
    Eigen::VectorXd _mutation(double f, const Eigen::MatrixXd& parent, size_t r1, size_t r2, size_t r3) const;
    Eigen::MatrixXd _crossover(double cr, const Eigen::MatrixXd& parent, const Eigen::MatrixXd& mutated);
    Eigen::MatrixXd _polynomial_mutation(const Eigen::MatrixXd para, double rate, size_t idx);
    std::vector<size_t> _seq_index(size_t) const;
    Eigen::MatrixXd _slice_matrix(const Eigen::MatrixXd&, const std::vector<size_t>&) const;
    Eigen::VectorXd _run_func(const Eigen::VectorXd&);        // wrapper of _func
    Eigen::MatrixXd _run_func_batch(const Eigen::MatrixXd&);  // wrapper of _func
    Eigen::MatrixXd _rand_matrix(const Eigen::VectorXd& lb, const Eigen::VectorXd& ub, size_t n);

    bool _dominate(const Eigen::VectorXd& obj1, const Eigen::VectorXd& obj2) const;
    std::vector<size_t> _extract_pf(const Eigen::MatrixXd&) const;
    std::vector<size_t> _extract_pf2(const Eigen::MatrixXd&) const;
    Eigen::VectorXi _dom_rank(const Eigen::MatrixXd& objs) const;
    Eigen::VectorXd _crowding_dist(const Eigen::MatrixXd& objs, const Eigen::VectorXi& ranks) const;
    Eigen::VectorXd _front_crowding_dist(const Eigen::MatrixXd&) const;
    std::vector<size_t> _nth_element(const Eigen::MatrixXd& objs, size_t) const;

public:
    typedef std::function<Eigen::VectorXd(const Eigen::VectorXd&)> ObjF;
    MOO(ObjF, size_t num_o, const Eigen::VectorXd& lb, const Eigen::VectorXd& ub);
    void set_np(size_t);
    void set_gen(size_t);
    void set_f(double);
    void set_cr(double);
    void set_seed(size_t);
    void set_record(bool);
    void set_anchor(const Eigen::MatrixXd&);
    void set_crowding_space(CrowdingSpace cs){_cs = cs; }
    size_t get_seed() const;
    void moo();
    Eigen::MatrixXd pareto_set() const;
    Eigen::MatrixXd pareto_front() const;

    Eigen::MatrixXd dbx() const;
    Eigen::MatrixXd dby() const;
    Eigen::MatrixXd anchor_x() const { return _anchor_x; }
    Eigen::MatrixXd anchor_y() const { return _anchor_y; }
    std::vector<size_t> nth_element(size_t) const;
    std::vector<size_t> sort() const;
    size_t best() const;

private:
    ObjF _func;

// implementation of MOEA/D
public:
    void moead();
    void moead_one_step();
    void set_moead_H(size_t v){_H = v; }
    void set_moead_T(size_t v){_T = v; }
    void set_moead_nr(size_t v){_nr = v; }
    void set_moead_delta(double v){_delta = v; }
protected:
    bool   _param_set = false;
    size_t _H         = 80;     // determine number of weights
    size_t _T         = 8;     // determine number of neighbours
    size_t _nr        = 1;
    double _delta     = 0.9;
    size_t _N;     // number of weight vectors
    Eigen::MatrixXi _B;
    Eigen::VectorXd _ref_point;
    Eigen::MatrixXd _lambdas; // weight vectors

    Eigen::MatrixXd _gen_weights() const;
    Eigen::MatrixXd _gen_weights(size_t num, size_t dim, double unit) const;
    void _setB();
    void _moead_initialize();
    std::vector<size_t> _select_mating_pool(size_t i);
    double _tchebysheff(const Eigen::VectorXd& obj_vals, const Eigen::VectorXd& weight) const;
};
