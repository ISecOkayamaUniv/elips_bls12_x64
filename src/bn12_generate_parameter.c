#include <ELiPS/bn12_generate_parameter.h>

void BN12_get_epsilon(){
    Fp inv,buf,result1,result2;
    Fp_init(&inv);
    Fp_init(&buf);
    Fp_init(&result1);
    Fp_init(&result2);
    
    Fp_set_ui(&buf,2);
    Fp_inv(&inv,&buf);
    mpn_sub_ui(buf.x0,prime,FPLIMB,3);
    
    Fp_sqrt(&buf,&buf);
    Fp_sub_ui(&buf,&buf,1);
    Fp_mul(&result1,&buf,&inv);
    Fp_mul(&result2,&result1,&result1);
    
    mpn_copyd(epsilon1,result1.x0,FPLIMB);
    mpn_copyd(epsilon2,result2.x0,FPLIMB);
}

void BN12_get_Two_inv(){
    mpn_set_ui(Two_inv,FPLIMB,2);
    mpn_invert(Two_inv,Two_inv,prime);
}

void BN12_set_basis(){
    Fp2_set_ui_ui(&Alpha_1,1);
    Fp2_inv(&Alpha_1_inv,&Alpha_1);
}

void BN12_set_frobenius_constant(){
	Fp2 tmp1,tmp2,tmp3;
	Fp2_init(&tmp1);
	Fp2_init(&tmp2);
	Fp2_init(&tmp3);
	
	mpz_t exp,buf,p2,p3,p4,p6,p8,p10;
	mpz_init(exp);
	mpz_init(buf);
	mpz_init(p2);
	mpz_init(p3);
	mpz_init(p4);
	mpz_init(p6);
	mpz_init(p8);
	mpz_init(p10);
	
	mpz_mul(p2,prime_z,prime_z);
	mpz_mul(p3,p2,prime_z);
	mpz_mul(p4,p3,prime_z);
	mpz_mul(p6,p4,p2);
	mpz_mul(p8,p6,p2);
	mpz_mul(p10,p8,p2);
	
	//frobenius_1
	mpz_sub_ui(exp,prime_z,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set f_p1
	Fp_set_ui(&frobenius_constant[f_p1][0].x0,1);
	Fp2_set(&frobenius_constant[f_p1][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p1][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p1][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p1][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p1][5],&tmp2,&tmp3);
	
	//set skew_f_p1
	Fp2_inv(&tmp1,&tmp1);
	mpz_sub_ui(exp,prime_z,1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp2,&Alpha_1,exp);
	Fp2_inv(&tmp2,&tmp2);
	Fp2_set(&skew_frobenius_constant[f_p1][0],&tmp1);
	Fp2_set(&skew_frobenius_constant[f_p1][1],&tmp2);
	
	//frobenius_2
	mpz_sub_ui(exp,p2,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set f_p2
	Fp_set_ui(&frobenius_constant[f_p2][0].x0,1);
	Fp2_set(&frobenius_constant[f_p2][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p2][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p2][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p2][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p2][5],&tmp2,&tmp3);
	//set skew_f_p2
	Fp2_inv(&tmp1,&tmp1);
	mpz_sub_ui(exp,p2,1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp2,&Alpha_1,exp);
	Fp2_inv(&tmp2,&tmp2);
	Fp2_set(&skew_frobenius_constant[f_p2][0],&tmp1);
	Fp2_set(&skew_frobenius_constant[f_p2][1],&tmp2);
	
	//frobenius_3
	mpz_sub_ui(exp,p3,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set f_p3
	Fp_set_ui(&frobenius_constant[f_p3][0].x0,1);
	Fp2_set(&frobenius_constant[f_p3][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p3][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p3][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p3][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p3][5],&tmp2,&tmp3);
	//set skew_f_p3
	Fp2_inv(&tmp1,&tmp1);
	mpz_sub_ui(exp,p3,1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp2,&Alpha_1,exp);
	Fp2_inv(&tmp2,&tmp2);
	Fp2_set(&skew_frobenius_constant[f_p3][0],&tmp1);
	Fp2_set(&skew_frobenius_constant[f_p3][1],&tmp2);
	
	//frobenius_constant[f_p4]
	mpz_sub_ui(exp,p4,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set frobenius_constant[f_p4]
	Fp_set_ui(&frobenius_constant[f_p4][0].x0,1);
	Fp2_set(&frobenius_constant[f_p4][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p4][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p4][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p4][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p4][5],&tmp2,&tmp3);
	
	//frobenius_constant[f_p8]
	mpz_sub_ui(exp,p8,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set frobenius_constant[f_p8]
	Fp_set_ui(&frobenius_constant[f_p8][0].x0,1);
	Fp2_set(&frobenius_constant[f_p8][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p8][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p8][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p8][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p8][5],&tmp2,&tmp3);
	
	//frobenius_10
	mpz_sub_ui(exp,p10,1);
	mpz_tdiv_q_ui(exp,exp,3);
	Fp2_pow(&tmp1,&Alpha_1,exp);
	Fp2_mul(&tmp2,&tmp1,&tmp1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp3,&Alpha_1,exp);
	//set frobenius_10
	Fp_set_ui(&frobenius_constant[f_p10][0].x0,1);
	Fp2_set(&frobenius_constant[f_p10][1],&tmp1);
	Fp2_set(&frobenius_constant[f_p10][2],&tmp2);
	Fp2_set(&frobenius_constant[f_p10][3],&tmp3);
	Fp2_mul(&frobenius_constant[f_p10][4],&tmp1,&tmp3);
	Fp2_mul(&frobenius_constant[f_p10][5],&tmp2,&tmp3);
	//set skew_f_10
	Fp2_inv(&tmp1,&tmp1);
	mpz_sub_ui(exp,p10,1);
	mpz_tdiv_q_ui(exp,exp,2);
	Fp2_pow(&tmp2,&Alpha_1,exp);
	Fp2_inv(&tmp2,&tmp2);
	Fp2_set(&skew_frobenius_constant[f_p10][0],&tmp1);
	Fp2_set(&skew_frobenius_constant[f_p10][1],&tmp2);
	
	
	//skew_frobenius_1
	mpz_sub_ui(exp,prime_z,1);
	mpz_mul_ui(exp,exp,2);
	Fp2_pow(&skew_frobenius_constant[f_p1][0],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p1][0],&skew_frobenius_constant[f_p1][0]);
	Fp2_mul(&skew_frobenius_constant[f_p1][0],&skew_frobenius_constant[f_p1][0],&frobenius_constant[f_p1][1]);
	mpz_sub_ui(exp,prime_z,1);
	mpz_mul_ui(exp,exp,3);
	Fp2_pow(&skew_frobenius_constant[f_p1][1],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p1][1],&skew_frobenius_constant[f_p1][1]);
	Fp2_mul(&skew_frobenius_constant[f_p1][1],&skew_frobenius_constant[f_p1][1],&frobenius_constant[f_p1][4]);
	
	//skew_frobenius_2
	mpz_sub_ui(exp,p2,1);
	mpz_mul_ui(exp,exp,2);
	Fp2_pow(&skew_frobenius_constant[f_p2][0],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p2][0],&skew_frobenius_constant[f_p2][0]);
	Fp2_mul(&skew_frobenius_constant[f_p2][0],&skew_frobenius_constant[f_p2][0],&frobenius_constant[f_p2][1]);
	mpz_sub_ui(exp,p2,1);
	mpz_mul_ui(exp,exp,3);
	Fp2_pow(&skew_frobenius_constant[f_p2][1],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p2][1],&skew_frobenius_constant[f_p2][1]);
	Fp2_mul(&skew_frobenius_constant[f_p2][1],&skew_frobenius_constant[f_p2][1],&frobenius_constant[f_p2][4]);
	
	//skew_frobenius_3
	mpz_sub_ui(exp,p3,1);
	mpz_mul_ui(exp,exp,2);
	Fp2_pow(&skew_frobenius_constant[f_p3][0],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p3][0],&skew_frobenius_constant[f_p3][0]);
	Fp2_mul(&skew_frobenius_constant[f_p3][0],&skew_frobenius_constant[f_p3][0],&frobenius_constant[f_p3][1]);
	mpz_sub_ui(exp,p3,1);
	mpz_mul_ui(exp,exp,3);
	Fp2_pow(&skew_frobenius_constant[f_p3][1],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p3][1],&skew_frobenius_constant[f_p3][1]);
	Fp2_mul(&skew_frobenius_constant[f_p3][1],&skew_frobenius_constant[f_p3][1],&frobenius_constant[f_p3][4]);
	
	//skew_frobenius_10
	mpz_sub_ui(exp,p10,1);
	mpz_mul_ui(exp,exp,2);
	Fp2_pow(&skew_frobenius_constant[f_p10][0],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p10][0],&skew_frobenius_constant[f_p10][0]);
	Fp2_mul(&skew_frobenius_constant[f_p10][0],&skew_frobenius_constant[f_p10][0],&frobenius_constant[f_p10][1]);
	mpz_sub_ui(exp,p10,1);
	mpz_mul_ui(exp,exp,3);
	Fp2_pow(&skew_frobenius_constant[f_p10][1],&Alpha_1,exp);
	Fp2_inv(&skew_frobenius_constant[f_p10][1],&skew_frobenius_constant[f_p10][1]);
	Fp2_mul(&skew_frobenius_constant[f_p10][1],&skew_frobenius_constant[f_p10][1],&frobenius_constant[f_p10][4]);
	
	mpz_clear(exp);
	mpz_clear(buf);
	mpz_clear(p2);
	mpz_clear(p3);
	mpz_clear(p4);
	mpz_clear(p6);
	mpz_clear(p8);
	mpz_clear(p10);
}

void BN12_set_curve_parameter(){
    mpn_set_ui(curve_b,FPLIMB,2);
}
