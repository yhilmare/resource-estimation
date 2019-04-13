//
// Created by Administrator on 2019/3/27.
//

#ifndef POSTGRESQL_GLOBAL_DEFINE_H
#define POSTGRESQL_GLOBAL_DEFINE_H

#define PG_HOST 10.69.35.174
#define PG_PASSWORD 123456
#define PG_DATABASE TPCD
#define PG_TIMEOUT 10
#define PG_USER ilmare

#define STOCK_PER_WAREHOUSE 156
#define DISTRICT_PER_WAREHOUSE 356
#define CUSTOMER_PER_DISTRICT 548
#define ORDER_PER_DISTRICT 4200

#define EXECUTE_TIME 1800

/* definitions for new order transaction */
#define MAX_NUM_ITEMS 15
#define MAX_ITEM_LEN 24
#define MAX_RETRY 3

#include <string>
#include "./lib/pg_lib/pg_prepared_statement.h"

const parameter_type type0[] = {int_type, int_type, int_type};
const parameter_type type1[] = {int_type, int_type};
const parameter_type type2[] = {int_type, int_type, int_type};
const parameter_type type3[] = {int_type, int_type, int_type, int_type, date_type, int_type, int_type};
const parameter_type type4[] = {int_type, int_type, int_type};
const parameter_type type5[] = {int_type};
const parameter_type type6[] = {int_type, int_type};
const parameter_type type7[] = {int_type, int_type, int_type};
const parameter_type type8[] = {int_type, int_type, int_type, int_type, int_type, int_type, int_type, numeric_type, text_type};
const parameter_type type9[] = {numeric_type, int_type};
const parameter_type type10[] = {int_type};
const parameter_type type11[] = {numeric_type, int_type, int_type};
const parameter_type type12[] = {int_type, int_type};
const parameter_type type13[] = {int_type, int_type, text_type};
const parameter_type type14[] = {int_type, int_type, text_type};
const parameter_type type15[] = {int_type, int_type, int_type};
const parameter_type type16[] = {int_type, int_type, int_type};
const parameter_type type17[] = {numeric_type, text_type, int_type, int_type, int_type};
const parameter_type type18[] = {numeric_type, int_type, int_type, int_type};
const parameter_type type19[] = {int_type, int_type, int_type, int_type, int_type, date_type, numeric_type, text_type};
const parameter_type type20[] = {int_type, int_type, text_type};
const parameter_type type21[] = {int_type, int_type, text_type};
const parameter_type type22[] = {int_type, int_type, int_type};
const parameter_type type23[] = {int_type, int_type, int_type, int_type, int_type, int_type};
const parameter_type type24[] = {int_type, int_type, int_type};
const parameter_type type25[] = {int_type, int_type};
const parameter_type type26[] = {int_type, int_type, int_type};
const parameter_type type27[] = {int_type, int_type, int_type};
const parameter_type type28[] = {int_type, int_type, int_type, int_type};
const parameter_type type29[] = {date_type, int_type, int_type, int_type};
const parameter_type type30[] = {int_type, int_type, int_type};
const parameter_type type31[] = {numeric_type, int_type, int_type, int_type};
const parameter_type type32[] = {int_type, int_type};
const parameter_type type33[] = {int_type, int_type, int_type, int_type};
const parameter_type type34[] = {int_type, int_type, int_type};

const static parameter_type *SQL_PARAMETER_TYPE[35] = {type0, type1, type2, type3, type4,
                                                type5, type6, type7, type8, type9,
                                                type10, type11, type12, type13, type14,
                                                type15, type16, type17, type18, type19,
                                                type20, type21, type22, type23, type24,
                                                type25, type26, type27, type28, type29,
                                                type30, type31, type32, type33, type34};

const std::string SQL_STRING[] = {
        "SELECT c_discount, c_last, c_credit, w_tax FROM customer, warehouse WHERE w_id = $1 AND c_w_id = w_id AND c_d_id = $2 AND c_id = $3",
        "SELECT d_next_o_id, d_tax FROM district WHERE d_id = $1 AND d_w_id = $2 FOR UPDATE",
        "UPDATE district SET d_next_o_id = $1 + 1 WHERE d_id = $2 AND d_w_id = $3",
        "INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local) VALUES($1, $2, $3, $4, $5, $6, $7)",
        "INSERT INTO new_orders (no_o_id, no_d_id, no_w_id) VALUES ($1,$2,$3)",
        "SELECT i_price, i_name, i_data FROM item WHERE i_id = $1",
        "SELECT s_quantity, s_data, s_dist_01, s_dist_02, s_dist_03, s_dist_04, s_dist_05, s_dist_06, s_dist_07, s_dist_08, s_dist_09, s_dist_10 FROM stock WHERE s_i_id = $1 AND s_w_id = $2 FOR UPDATE",
        "UPDATE stock SET s_quantity = $1 WHERE s_i_id = $2 AND s_w_id = $3",
        "INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_dist_info) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)",
        "UPDATE warehouse SET w_ytd = w_ytd + $1 WHERE w_id = $2",
        "SELECT w_street_1, w_street_2, w_city, w_state, w_zip, w_name FROM warehouse WHERE w_id = $1",
        "UPDATE district SET d_ytd = d_ytd + $1 WHERE d_w_id = $2 AND d_id = $3",
        "SELECT d_street_1, d_street_2, d_city, d_state, d_zip, d_name FROM district WHERE d_w_id = $1 AND d_id = $2",
        "SELECT count(c_id) FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3",
        "SELECT c_id FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3 ORDER BY c_first",
        "SELECT c_first, c_middle, c_last, c_street_1, c_street_2, c_city, c_state, c_zip, c_phone, c_credit, c_credit_lim, c_discount, c_balance, c_since FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3 FOR UPDATE",
        "SELECT c_data FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3",
        "UPDATE customer SET c_balance = $1, c_data = $2 WHERE c_w_id = $3 AND c_d_id = $4 AND c_id = $5",
        "UPDATE customer SET c_balance = $1 WHERE c_w_id = $2 AND c_d_id = $3 AND c_id = $4",
        "INSERT INTO history(h_c_d_id, h_c_w_id, h_c_id, h_d_id, h_w_id, h_date, h_amount, h_data) VALUES($1, $2, $3, $4, $5, $6, $7, $8)",
        "SELECT count(c_id) FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3",
        "SELECT c_balance, c_first, c_middle, c_last FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3 ORDER BY c_first",
        "SELECT c_balance, c_first, c_middle, c_last FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3",
        "SELECT o_id, o_entry_d, COALESCE(o_carrier_id,0) FROM orders WHERE o_w_id = $1 AND o_d_id = $2 AND o_c_id = $3 AND o_id = (SELECT MAX(o_id) FROM orders WHERE o_w_id = $4 AND o_d_id = $5 AND o_c_id = $6)",
        "SELECT ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_delivery_d FROM order_line WHERE ol_w_id = $1 AND ol_d_id = $2 AND ol_o_id = $3",
        "SELECT COALESCE(MIN(no_o_id),0) FROM new_orders WHERE no_d_id = $1 AND no_w_id = $2",
        "DELETE FROM new_orders WHERE no_o_id = $1 AND no_d_id = $2 AND no_w_id = $3",
        "SELECT o_c_id FROM orders WHERE o_id = $1 AND o_d_id = $2 AND o_w_id = $3",
        "UPDATE orders SET o_carrier_id = $1 WHERE o_id = $2 AND o_d_id = $3 AND o_w_id = $4",
        "UPDATE order_line SET ol_delivery_d = $1 WHERE ol_o_id = $2 AND ol_d_id = $3 AND ol_w_id = $4",
        "SELECT SUM(ol_amount) FROM order_line WHERE ol_o_id = $1 AND ol_d_id = $2 AND ol_w_id = $3",
        "UPDATE customer SET c_balance = c_balance + $1 , c_delivery_cnt = c_delivery_cnt + 1 WHERE c_id = $2 AND c_d_id = $3 AND c_w_id = $4",
        "SELECT d_next_o_id FROM district WHERE d_id = $1 AND d_w_id = $2",
        "SELECT DISTINCT ol_i_id FROM order_line WHERE ol_w_id = $1 AND ol_d_id = $2 AND ol_o_id < $3 AND ol_o_id >= ($4 - 20)",
        "SELECT count(*) FROM stock WHERE s_w_id = $1 AND s_i_id = $2 AND s_quantity < $3"
};

#endif //POSTGRESQL_GLOBAL_DEFINE_H
