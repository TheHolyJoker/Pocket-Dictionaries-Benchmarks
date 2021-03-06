//
// Created by tomer on 9/19/20.
//


//Only the skeleton.
#ifndef CLION_CODE_VALIDATION_TESTS_H
#define CLION_CODE_VALIDATION_TESTS_H


#include <random>
#include <tuple>


template<class pd512_type>
auto validate_PD_core(size_t reps, pd512_type *d, bool to_print, float load_factor = 1, size_t m = 50, size_t f = 51, size_t l = 8) -> bool{
        auto naive_d = naive_PD(m, f, l);
        vector<int> in_q(0), out_q(f), in_r(0), out_r(f);

        bool same_res = true;

        for (size_t i = 0; i < f; ++i) {
            out_q[i] = rand() % m;
            out_r[i] = rand() % SL(l - 1);
        }
        if (load_factor != 0) {
            int lim = floor(f * load_factor);
            for (int i = 0; i < lim; ++i) {
                size_t index = rand() % out_q.size();
                size_t q = out_q[index], r = out_r[index];
                if (to_print) { cout << "in: " << q << ", " << r << endl; }

                d->insert(q, r);
                naive_d.insert(q, r);
                in_q.push_back(q);
                in_r.push_back(r);
                out_q.erase(out_q.begin() + index);
                out_r.erase(out_r.begin() + index);
            }
        }
        for (size_t i = 0; i < reps; ++i) {
            if (to_print) {
                cout << *d;
                cout << endl;
                cout << i << "\t";
            }
            if (rand() % 2) {
                if (out_q.empty()) {
                    if (to_print) { cout << "emp" << endl; }
                    continue;
                }
                size_t index = rand() % out_q.size();
                size_t q = out_q[index], r = out_r[index];
                if (to_print) { cout << "in: " << q << ", " << r << endl; }

                same_res = (d->lookup(q, r) == naive_d.lookup(q, r));
                if (!same_res) {
                    break_point_helper();
                    cout << "case 1 fail " << i << endl;
                    return false;
                }
                /*    d->lookup(q, r);
                    auto it = find(in_q.begin(), in_q.end(), q);
                    if (it != in_q.end()) {
                        size_t index2 = distance(in_q.begin(), it);
                        if (in_q[index2] != r) {
                            break_point_helper();
                            cout << "case 1 fail " << i << endl;
                            return false;
                        }
                    } else {
                        break_point_helper();
                        cout << "case 1 fail " << i << endl;
                        return false;
                    }*/
                d->insert(q, r);
                naive_d.insert(q, r);
                same_res = (d->lookup(q, r) == naive_d.lookup(q, r));
                if (!same_res) {
                    break_point_helper();
                    d->lookup(q, r);
                    cout << "case 2 fail " << i << endl;
                    return false;
                }

                in_q.push_back(q);
                in_r.push_back(r);

                out_q.erase(out_q.begin() + index);
                out_r.erase(out_r.begin() + index);


            } else {
                if (in_q.empty()) {
                    if (to_print) { cout << "emp" << endl; }
                    continue;
                }
                size_t index = rand() % in_q.size();
                size_t q = in_q[index], r = in_r[index];
                if (to_print) { cout << "del: " << q << ", " << r << endl; }
//            bool BPC = (q == 105) and (i == 6);
                same_res = (d->lookup(q, r) == naive_d.lookup(q, r));
                if (!same_res) {
                    break_point_helper();
                    d->lookup(q, r);
                    cout << "case 3 fail " << i << endl;
                    return false;
                }


                d->remove(q, r);
                naive_d.remove(q, r);

                same_res = (d->lookup(q, r) == naive_d.lookup(q, r));
                if (!same_res) {
                    auto a_res = d->lookup(q, r);
                    bool v_res = naive_d.lookup(q, r);
                    break_point_helper();
                    d->lookup(q, r);
                    cout << "case 4 fail " << i << endl;
                    return false;
                }
                out_q.push_back(q);
                out_r.push_back(r);

                in_q.erase(in_q.begin() + index);
                in_r.erase(in_r.begin() + index);
            }
        }
        if (to_print) { cout << "Validation of template pd passed." << endl; }
        return true;

    }


}


namespace namePD {
    void break_point_helper();

    auto validate_header_get_interval_function(size_t reps) -> bool;

    auto validate_get_interval_function_constant(size_t reps) -> bool;

    auto validate_push_function(size_t reps) -> bool;

    auto validate_pull_function(size_t reps) -> bool;

    auto validate_rank_function(size_t reps) -> bool;

    /**
 * smashing stack for some reason.
 * @param reps
 * @return
 */
    auto validate_header_type(size_t reps) -> bool;

    auto t1(bool to_print = false) -> bool;

    auto t2(bool to_print = false) -> bool;

    auto t3(bool to_print = false) -> bool;

    auto t4(bool to_print = false) -> bool;

    auto t5(size_t m, size_t f, size_t l, bool to_print = false) -> bool;


    template<class T>
    auto validate_CPD(size_t reps, bool to_seed, bool to_print = 0) -> bool;


    template<class T>
    auto validate_PD(size_t reps, bool to_seed, bool to_print = 0) -> bool;

    template<class T>
    auto validate_PD_higher_load(size_t reps, float load_factor, bool to_seed, bool to_print = 0) -> bool;

    /**
 * random tests.
 * @param reps
 * @param to_print
 * @return
 */
    auto validate_PD_single_run(size_t reps, bool to_print = 0) -> bool;

    auto validate_const_PD(size_t reps, bool to_print = 0) -> bool;

    template<class T>
    auto validate_PD_core(size_t reps, T *d, size_t m, size_t f, size_t l, bool to_print, float load_factor) -> bool;

    auto r0_core(size_t reps, PD *d, size_t m, size_t f, size_t l, bool to_print = 0) -> bool;


    auto
    validate_PD_by_load_factor(size_t reps, float load_factor, size_t m = 32, size_t f = 32, bool to_print = 0) -> bool;

    auto validate_safe_PD(size_t reps, bool to_print = 0) -> bool;

    auto validate_safe_PD_const_case(size_t reps, bool to_print = 0) -> bool;

    auto naive_pd_r0(size_t reps, bool to_print = 0) -> bool;

    auto r1(size_t reps, bool to_print) -> bool;

    auto vector_rw_t1() -> bool;

    auto pow2c_r0(size_t reps, bool to_print = 0) -> bool;

    auto pow2c_naive_r0(size_t reps, bool to_print = 0) -> bool;


    //bool naive_pd_r0(size_t reps, bool to_print = 0) ->bool;
}// namespace namePD
#endif//CLION_CODE_VALIDATION_TESTS_H
