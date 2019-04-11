//
// Created by Administrator on 2019/4/11.
//

#include "neword.h"
#include "../global_define.h"
#include <random>

int neword(int t_num, int w_id_arg, int d_id_arg,
        int c_id_arg, int o_ol_cnt_arg, int o_all_local_arg,
        int itemid[], int supware[], int qty[], pg_connection &con,
        std::vector<pg_prepared_statement> &val) {

    int w_id = w_id_arg;
    int d_id = d_id_arg;
    int c_id = c_id_arg;
    int o_ol_cnt = o_ol_cnt_arg;
    int o_all_local = o_all_local_arg;
    float c_discount;
    char c_last[17];
    char c_credit[3];
    float w_tax;
    int d_next_o_id;
    float d_tax;
    PG::Date current;
    long seconds = current.get_million_seconds();
    extern std::default_random_engine e;
    std::uniform_int_distribution<long> d3(0L, 630720000L);
    int o_id;
    char i_name[25];
    float i_price;
    char i_data[51];
    int ol_i_id;
    int s_quantity;
    char s_data[51];
    char s_dist_01[25];
    char s_dist_02[25];
    char s_dist_03[25];
    char s_dist_04[25];
    char s_dist_05[25];
    char s_dist_06[25];
    char s_dist_07[25];
    char s_dist_08[25];
    char s_dist_09[25];
    char s_dist_10[25];
    char ol_dist_info[25];
    int ol_supply_w_id;
    float ol_amount;
    int ol_number;
    int ol_quantity;

    char iname[MAX_NUM_ITEMS][MAX_ITEM_LEN];
    char bg[MAX_NUM_ITEMS];
    float amt[MAX_NUM_ITEMS];
    float price[MAX_NUM_ITEMS];
    int stock[MAX_NUM_ITEMS];
    float total = 0.0;

    int min_num;
    int i,j,tmp,swp;
    int ol_num_seq[MAX_NUM_ITEMS];
    int proceed = 0;

    try{
        proceed = 1;
        con.set_auto_commit(false);
        pg_prepared_statement st = val[0];
        /*
         * SELECT c_discount, c_last, c_credit, w_tax FROM customer, warehouse WHERE w_id = $1 AND c_w_id = w_id AND c_d_id = $2 AND c_id = $3
         * const parameter_type type0[] = {int_type, int_type, int_type};
         * */
        st.set_int(0, w_id);
        st.set_int(1, d_id);
        st.set_int(2, c_id);

        pg_resultset res = st.execute_query();
        column[0].buffer_type = MYSQL_TYPE_FLOAT;
        column[0].buffer = &c_discount;
        column[1].buffer_type = MYSQL_TYPE_STRING;
        column[1].buffer = c_last;
        column[1].buffer_length = sizeof(c_last);
        column[2].buffer_type = MYSQL_TYPE_STRING;
        column[2].buffer = c_credit;
        column[2].buffer_length = sizeof(c_credit);
        column[3].buffer_type = MYSQL_TYPE_FLOAT;
        column[3].buffer = &w_tax;
        con.commit();
    }catch (const std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
    }
    try{
        proceed = 2;
        con.set_auto_commit(false);
        pg_prepared_statement st = val[1];
        /*
         * const parameter_type type1[] = {int_type, int_type};
         * "SELECT d_next_o_id, d_tax FROM district WHERE d_id = $1 AND d_w_id = $2 FOR UPDATE",
         * */
        st.set_int(0, d_id);
        st.set_int(1, w_id);

        pg_resultset res = st.execute_query();
        con.commit();
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
    }

    try{
        proceed = 3;
        con.set_auto_commit(false);
        pg_prepared_statement st = val[2];
        /*
         * const parameter_type type2[] = {int_type, int_type, int_type};
         * "UPDATE district SET d_next_o_id = $1 + 1 WHERE d_id = $2 AND d_w_id = $3",
         * */
        st.set_int(0, d_next_o_id);
        st.set_int(1, d_id);
        st.set_int(2, w_id);

        st.execute_update();
        con.commit();
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
    }

    try{
        proceed = 4;
        con.set_auto_commit(false);
        pg_prepared_statement st = val[3];
        /*
         * const parameter_type type3[] = {int_type, int_type, int_type, int_type, date_type, int_type, int_type};
         * "INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local) VALUES($1, $2, $3, $4, $5, $6, $7)",
         * */
        st.set_int(0, o_id);
        st.set_int(1, d_id);
        st.set_int(2, w_id);
        st.set_int(3, c_id);
        PG::Date date(seconds - d3(e));
        st.set_date(4, date);
        st.set_int(5, o_ol_cnt);
        st.set_int(6, o_all_local);

        st.execute_update();
        con.commit();
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
    }

    try{
        proceed = 5;
        con.set_auto_commit(false);
        pg_prepared_statement st = val[4];
        /*
         * {int_type, int_type, int_type};
         * "INSERT INTO new_orders (no_o_id, no_d_id, no_w_id) VALUES ($1,$2,$3)",
         * */
        st.set_int(0, o_id);
        st.set_int(1, d_id);
        st.set_int(2, w_id);

        st.execute_update();
        con.commit();
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
    }

    /* sort orders to avoid DeadLock */
    for (i = 0; i < o_ol_cnt; i++) {
        ol_num_seq[i] = i;
    }
    for (i = 0; i < (o_ol_cnt - 1); i++) {
        tmp = (MAXITEMS + 1) * supware[ol_num_seq[i]] + itemid[ol_num_seq[i]];
        min_num = i;
        for ( j = i+1; j < o_ol_cnt; j++) {
            if ( (MAXITEMS + 1) * supware[ol_num_seq[j]] + itemid[ol_num_seq[j]] < tmp ){
                tmp = (MAXITEMS + 1) * supware[ol_num_seq[j]] + itemid[ol_num_seq[j]];
                min_num = j;
            }
        }
        if ( min_num != i ){
            swp = ol_num_seq[min_num];
            ol_num_seq[min_num] = ol_num_seq[i];
            ol_num_seq[i] = swp;
        }
    }

    for (ol_number = 1; ol_number <= o_ol_cnt; ol_number++) {
        ol_supply_w_id = supware[ol_num_seq[ol_number - 1]];
        ol_i_id = itemid[ol_num_seq[ol_number - 1]];
        ol_quantity = qty[ol_num_seq[ol_number - 1]];

        /* EXEC SQL WHENEVER NOT FOUND GOTO invaliditem; */
        proceed = 6;
        /*EXEC_SQL SELECT i_price, i_name, i_data
            INTO :i_price, :i_name, :i_data
                FROM item
                WHERE i_id = :ol_i_id;*/
        mysql_stmt = stmt[t_num][5];

        memset(param, 0, sizeof(MYSQL_BIND) * 1); /* initialize */
        param[0].buffer_type = MYSQL_TYPE_LONG;
        param[0].buffer = &ol_i_id;
        if( mysql_stmt_bind_param(mysql_stmt, param) ) goto sqlerr;
        if( mysql_stmt_execute(mysql_stmt) ) goto sqlerr;

        if( mysql_stmt_store_result(mysql_stmt) ) goto sqlerr;
        memset(column, 0, sizeof(MYSQL_BIND) * 3); /* initialize */
        column[0].buffer_type = MYSQL_TYPE_FLOAT;
        column[0].buffer = &i_price;
        column[1].buffer_type = MYSQL_TYPE_STRING;
        column[1].buffer = i_name;
        column[1].buffer_length = sizeof(i_name);
        column[2].buffer_type = MYSQL_TYPE_STRING;
        column[2].buffer = i_data;
        column[2].buffer_length = sizeof(i_data);
        if( mysql_stmt_bind_result(mysql_stmt, column) ) goto sqlerr;
        switch( mysql_stmt_fetch(mysql_stmt) ) {
            case 0: //SUCCESS
            case MYSQL_DATA_TRUNCATED:
                break;

            case MYSQL_NO_DATA: //NO MORE DATA
                mysql_stmt_free_result(mysql_stmt);
                goto invaliditem;

            case 1: //ERROR
            default:
                mysql_stmt_free_result(mysql_stmt);
                goto sqlerr;
        }
        mysql_stmt_free_result(mysql_stmt);


        price[ol_num_seq[ol_number - 1]] = i_price;
        strncpy(iname[ol_num_seq[ol_number - 1]], i_name, 25);

        /* EXEC SQL WHENEVER NOT FOUND GOTO sqlerr; */

#ifdef DEBUG
        printf("n %d\n",proceed);
#endif

        proceed = 7;
        /*EXEC_SQL SELECT s_quantity, s_data, s_dist_01, s_dist_02,
                        s_dist_03, s_dist_04, s_dist_05, s_dist_06,
                        s_dist_07, s_dist_08, s_dist_09, s_dist_10
            INTO :s_quantity, :s_data, :s_dist_01, :s_dist_02,
                     :s_dist_03, :s_dist_04, :s_dist_05, :s_dist_06,
                     :s_dist_07, :s_dist_08, :s_dist_09, :s_dist_10
                FROM stock
                WHERE s_i_id = :ol_i_id
            AND s_w_id = :ol_supply_w_id
            FOR UPDATE;*/
        mysql_stmt = stmt[t_num][6];

        memset(param, 0, sizeof(MYSQL_BIND) * 2); /* initialize */
        param[0].buffer_type = MYSQL_TYPE_LONG;
        param[0].buffer = &ol_i_id;
        param[1].buffer_type = MYSQL_TYPE_LONG;
        param[1].buffer = &ol_supply_w_id;
        if( mysql_stmt_bind_param(mysql_stmt, param) ) goto sqlerr;
        if( mysql_stmt_execute(mysql_stmt) ) goto sqlerr;

        if( mysql_stmt_store_result(mysql_stmt) ) goto sqlerr;
        memset(column, 0, sizeof(MYSQL_BIND) * 12); /* initialize */
        column[0].buffer_type = MYSQL_TYPE_LONG;
        column[0].buffer = &s_quantity;
        column[1].buffer_type = MYSQL_TYPE_STRING;
        column[1].buffer = s_data;
        column[1].buffer_length = sizeof(s_data);
        column[2].buffer_type = MYSQL_TYPE_STRING;
        column[2].buffer = s_dist_01;
        column[2].buffer_length = sizeof(s_dist_01);
        column[3].buffer_type = MYSQL_TYPE_STRING;
        column[3].buffer = s_dist_02;
        column[3].buffer_length = sizeof(s_dist_02);
        column[4].buffer_type = MYSQL_TYPE_STRING;
        column[4].buffer = s_dist_03;
        column[4].buffer_length = sizeof(s_dist_03);
        column[5].buffer_type = MYSQL_TYPE_STRING;
        column[5].buffer = s_dist_04;
        column[5].buffer_length = sizeof(s_dist_04);
        column[6].buffer_type = MYSQL_TYPE_STRING;
        column[6].buffer = s_dist_05;
        column[6].buffer_length = sizeof(s_dist_05);
        column[7].buffer_type = MYSQL_TYPE_STRING;
        column[7].buffer = s_dist_06;
        column[7].buffer_length = sizeof(s_dist_06);
        column[8].buffer_type = MYSQL_TYPE_STRING;
        column[8].buffer = s_dist_07;
        column[8].buffer_length = sizeof(s_dist_07);
        column[9].buffer_type = MYSQL_TYPE_STRING;
        column[9].buffer = s_dist_08;
        column[9].buffer_length = sizeof(s_dist_08);
        column[10].buffer_type = MYSQL_TYPE_STRING;
        column[10].buffer = s_dist_09;
        column[10].buffer_length = sizeof(s_dist_09);
        column[11].buffer_type = MYSQL_TYPE_STRING;
        column[11].buffer = s_dist_10;
        column[11].buffer_length = sizeof(s_dist_10);
        if( mysql_stmt_bind_result(mysql_stmt, column) ) goto sqlerr;
        switch( mysql_stmt_fetch(mysql_stmt) ) {
            case 0: //SUCCESS
                break;
            case 1: //ERROR
            case MYSQL_NO_DATA: //NO MORE DATA
            default:
                mysql_stmt_free_result(mysql_stmt);
                goto sqlerr;
        }
        mysql_stmt_free_result(mysql_stmt);


        pick_dist_info(ol_dist_info, d_id);	/* pick correct
							 * s_dist_xx */

        stock[ol_num_seq[ol_number - 1]] = s_quantity;

        if ((strstr(i_data, "original") != NULL) &&
            (strstr(s_data, "original") != NULL))
            bg[ol_num_seq[ol_number - 1]] = 'B';
        else
            bg[ol_num_seq[ol_number - 1]] = 'G';

        if (s_quantity > ol_quantity)
            s_quantity = s_quantity - ol_quantity;
        else
            s_quantity = s_quantity - ol_quantity + 91;

#ifdef DEBUG
        printf("n %d\n",proceed);
#endif

        proceed = 8;
        /*EXEC_SQL UPDATE stock SET s_quantity = :s_quantity
                WHERE s_i_id = :ol_i_id
            AND s_w_id = :ol_supply_w_id;*/
        mysql_stmt = stmt[t_num][7];

        memset(param, 0, sizeof(MYSQL_BIND) * 3); /* initialize */
        param[0].buffer_type = MYSQL_TYPE_LONG;
        param[0].buffer = &s_quantity;
        param[1].buffer_type = MYSQL_TYPE_LONG;
        param[1].buffer = &ol_i_id;
        param[2].buffer_type = MYSQL_TYPE_LONG;
        param[2].buffer = &ol_supply_w_id;
        if( mysql_stmt_bind_param(mysql_stmt, param) ) goto sqlerr;
        if( mysql_stmt_execute(mysql_stmt) ) goto sqlerr;


        ol_amount = ol_quantity * i_price * (1 + w_tax + d_tax) * (1 - c_discount);
        amt[ol_num_seq[ol_number - 1]] = ol_amount;
        total += ol_amount;

#ifdef DEBUG
        printf("n %d\n",proceed);
#endif

        proceed = 9;
        /*EXEC_SQL INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id,
                         ol_number, ol_i_id,
                         ol_supply_w_id, ol_quantity,
                         ol_amount, ol_dist_info)
            VALUES (:o_id, :d_id, :w_id, :ol_number, :ol_i_id,
                :ol_supply_w_id, :ol_quantity, :ol_amount,
                :ol_dist_info);*/
        mysql_stmt = stmt[t_num][8];

        memset(param, 0, sizeof(MYSQL_BIND) * 9); /* initialize */
        param[0].buffer_type = MYSQL_TYPE_LONG;
        param[0].buffer = &o_id;
        param[1].buffer_type = MYSQL_TYPE_LONG;
        param[1].buffer = &d_id;
        param[2].buffer_type = MYSQL_TYPE_LONG;
        param[2].buffer = &w_id;
        param[3].buffer_type = MYSQL_TYPE_LONG;
        param[3].buffer = &ol_number;
        param[4].buffer_type = MYSQL_TYPE_LONG;
        param[4].buffer = &ol_i_id;
        param[5].buffer_type = MYSQL_TYPE_LONG;
        param[5].buffer = &ol_supply_w_id;
        param[6].buffer_type = MYSQL_TYPE_LONG;
        param[6].buffer = &ol_quantity;
        param[7].buffer_type = MYSQL_TYPE_FLOAT;
        param[7].buffer = &ol_amount;
        param[8].buffer_type = MYSQL_TYPE_STRING;
        param[8].buffer = ol_dist_info;
        param[8].buffer_length = strlen(ol_dist_info);
        if( mysql_stmt_bind_param(mysql_stmt, param) ) goto sqlerr;
        if( mysql_stmt_execute(mysql_stmt) ) goto sqlerr;


    }			/* End Order Lines */

#ifdef DEBUG
    printf("insert 3\n");
	fflush(stdout);
#endif

    /*EXEC_SQL COMMIT WORK;*/
    if( mysql_commit(ctx[t_num]) ) goto sqlerr;
    clk1 = clock_gettime(CLOCK_REALTIME, &tbuf1 );
    if (ftrx_file) {
        fprintf(ftrx_file,"t_num: %d finish: %lu %lu start: %lu %lu\n",t_num, tbuf1.tv_sec, tbuf1.tv_nsec,
                tbuf_start.tv_sec, tbuf_start.tv_nsec);
    }

    return (1);

    invaliditem:
    /*EXEC_SQL ROLLBACK WORK;*/
    mysql_rollback(ctx[t_num]);

    /* printf("Item number is not valid\n"); */
    return (1); /* OK? */

    sqlerr:
    fprintf(stderr,"neword %d:%d\n",t_num,proceed);
    error(ctx[t_num],mysql_stmt);
    /*EXEC SQL WHENEVER SQLERROR GOTO sqlerrerr;*/
    /*EXEC_SQL ROLLBACK WORK;*/
    mysql_rollback(ctx[t_num]);
    sqlerrerr:
    return (0);
}
