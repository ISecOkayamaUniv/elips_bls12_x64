#include <ELiPS/bn12_test.h>
int BN12_test_rational_point(){
    printf("====================================================================================\n");
    EFp12 test_G1,test_G2;
    EFp12_init(&test_G1);
    EFp12_init(&test_G2);
    
    BN12_EFp12_generate_G1(&test_G1);
    EFp12_printf("G1\n",&test_G1);
    printf("\n");
    EFp12_SCM(&test_G1,&test_G1,order_z);
    EFp12_printf("G1 test\n",&test_G1);
    printf("\n");
    
    EFp12_generate_G2(&test_G2);
    EFp12_printf("G2\n",&test_G2);
    printf("\n");
    EFp12_SCM(&test_G2,&test_G2,order_z);
    EFp12_printf("G2 test\n",&test_G2);
    printf("\n");

    if(test_G1.infinity!=1 || test_G2.infinity!=1) return 1;
    else return 0;
}

void BN12_test_plain_ate_pairing(){
    printf("====================================================================================\n");
    printf("Plain-ate pairing\n\n");
    EFp12 P,Q,s1P,s2P,s1Q,s2Q;
    EFp12_init(&P);
    EFp12_init(&Q);
    EFp12_init(&s1P);
    EFp12_init(&s2P);
    EFp12_init(&s1Q);
    EFp12_init(&s2Q);
    Fp12 Z,test1,test2,test3;
    Fp12_init(&Z);
    Fp12_init(&test1);
    Fp12_init(&test2);
    Fp12_init(&test3);
    mpz_t s12,s1,s2;
    mpz_init(s12);
    mpz_init(s1);
    mpz_init(s2);
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));
    mpz_urandomm(s1,state,order_z);
	mpz_urandomm(s2,state,order_z);
    mpz_mul(s12,s1,s2);
    mpz_mod(s12,s12,order_z);
    
    BN12_EFp12_generate_G1(&P);
    EFp12_printf("P\n",&P);
    printf("\n\n");
    EFp12_generate_G2(&Q);
    EFp12_printf("Q\n",&Q);
    printf("\n\n");
    EFp12_SCM(&s1P,&P,s1);
    EFp12_SCM(&s2P,&P,s2);
    EFp12_SCM(&s1Q,&Q,s1);
    EFp12_SCM(&s2Q,&Q,s2);
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test1\n\n");
    printf("plain_ate(P,Q)^s1*s2\n");
    BN12_Plain_ate_pairing(&Z,&P,&Q);
    Fp12_pow(&test1,&Z,s12);
    printf("Miller's Algo. (Plain-ate) : %.2f[ms]\n",MILLER_PLAINATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_PLAINATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test1);
    printf("\n\n");
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test2\n\n");
    printf("plain_ate([s1]P,[s2Q])\n");
    BN12_Plain_ate_pairing(&test2,&s1P,&s2Q);
    printf("Miller's Algo. (Plain-ate) : %.2f[ms]\n",MILLER_PLAINATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_PLAINATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test2);
    printf("\n\n");
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test3\n\n");
    printf("plain_ate([s2]P,[s1]Q)\n");
    BN12_Plain_ate_pairing(&test3,&s2P,&s1Q);
    printf("Miller's Algo. (Plain-ate) : %.2f[ms]\n",MILLER_PLAINATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_PLAINATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test3);
    printf("\n\n");
    
    printf("bilinear test\n");
    if(Fp12_cmp(&test1,&test2)==0 && Fp12_cmp(&test1,&test3)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
    
    mpz_clear(s12);
    mpz_clear(s1);
    mpz_clear(s2);
}

int BN12_test_opt_ate_pairing(int pairing){
    int i,n=0;
    float opt_time=0,opt_compress_time=0,opt_compress_lazy_time=0,opt_lazy_time=0;
    struct timeval tv_A,tv_B;
    printf("====================================================================================\n");
    printf("BN12_Opt-ate pairing\n\n");

    EFp12 P,Q,s1P,s2P,s1Q,s2Q;
    EFp12_init(&P);
    EFp12_init(&Q);
    EFp12_init(&s1P);
    EFp12_init(&s2P);
    EFp12_init(&s1Q);
    EFp12_init(&s2Q);

    Fp12 Z,testA,testB,testC,test1,test2,test3,test4;
    Fp12_init(&Z);
    Fp12_init(&testA);
    Fp12_init(&testB);
    Fp12_init(&testC);
    Fp12_init(&test1);
    Fp12_init(&test2);
    Fp12_init(&test3);
    Fp12_init(&test4);

    mpz_t s12,s1,s2;
    mpz_init(s12);
    mpz_init(s1);
    mpz_init(s2);

    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));

    mpz_urandomm(s1,state,order_z);
    mpz_urandomm(s2,state,order_z);
    mpz_mul(s12,s1,s2);
    mpz_mod(s12,s12,order_z);

    BN12_EFp12_generate_G1(&P);
    EFp12_generate_G2(&Q);

    EFp12_SCM(&s1P,&P,s1);
    EFp12_SCM(&s2P,&P,s2);
    EFp12_SCM(&s1Q,&Q,s1);
    EFp12_SCM(&s2Q,&Q,s2);

    
    BN12_Opt_ate_pairing(&Z,&P,&Q);
    Fp12_pow(&testA,&Z,s12);
    BN12_Opt_ate_pairing(&testB,&s1P,&s2Q);    
    BN12_Opt_ate_pairing(&testC,&s2P,&s1Q);
    
    printf("bilinear test\n");
    if(Fp12_cmp(&testA,&testB)!=0 && Fp12_cmp(&testA,&testC)!=0){
        printf("bilinear failed!!\n\n");
	return 1;
    }


for(i=0;i<pairing;i++){

    BN12_EFp12_generate_G1(&P);
    EFp12_generate_G2(&Q);

    gettimeofday(&tv_A,NULL);
    BN12_Opt_ate_pairing(&test1,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    BN12_Opt_ate_pairing_lazy(&test2,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_lazy_time+=timedifference_msec(tv_A,tv_B);
    
    
    if(Fp12_cmp(&test1,&test2)!=0){
        printf("failed!\n\n");
	    Fp12_printf("",&test1);
    	Fp12_printf("\n",&test2);
    	printf("\n\n");
    	return 1;
    }
}

    printf("BN12 opt ate.                : %.4f[ms]\n",opt_time/pairing);
    printf("BN12 opt ate lazy.           : %.4f[ms]\n",opt_lazy_time/pairing);

    mpz_clear(s12);
    mpz_clear(s1);
    mpz_clear(s2);

    return 0;
}

void BN12_test_x_ate_pairing(){
    printf("====================================================================================\n");
    printf("X-ate pairing\n\n");
    EFp12 P,Q,s1P,s2P,s1Q,s2Q;
    EFp12_init(&P);
    EFp12_init(&Q);
    EFp12_init(&s1P);
    EFp12_init(&s2P);
    EFp12_init(&s1Q);
    EFp12_init(&s2Q);
    Fp12 Z,test1,test2,test3;
    Fp12_init(&Z);
    Fp12_init(&test1);
    Fp12_init(&test2);
    Fp12_init(&test3);
    mpz_t s12,s1,s2;
    mpz_init(s12);
    mpz_init(s1);
    mpz_init(s2);
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));
    mpz_urandomm(s1,state,order_z);
	mpz_urandomm(s2,state,order_z);
    mpz_mul(s12,s1,s2);
    mpz_mod(s12,s12,order_z);
    
    BN12_EFp12_generate_G1(&P);
    EFp12_printf("P\n",&P);
    printf("\n\n");
    EFp12_generate_G2(&Q);
    EFp12_printf("Q\n",&Q);
    printf("\n\n");
    EFp12_SCM(&s1P,&P,s1);
    EFp12_SCM(&s2P,&P,s2);
    EFp12_SCM(&s1Q,&Q,s1);
    EFp12_SCM(&s2Q,&Q,s2);
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test1\n\n");
    printf("x_ate(P,Q)^s1*s2\n");
    BN12_X_ate_pairing(&Z,&P,&Q);
    Fp12_pow(&test1,&Z,s12);
    printf("Miller's Algo. (X-ate) : %.2f[ms]\n",MILLER_XATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_XATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test1);
    printf("\n\n");
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test2\n\n");
    printf("x_ate([s1]P,[s2]Q)\n");
    BN12_X_ate_pairing(&test2,&s1P,&s2Q);
    printf("Miller's Algo. (X-ate) : %.2f[ms]\n",MILLER_XATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_XATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test2);
    printf("\n\n");
    
    printf("------------------------------------------------------------------------------------\n");
    printf("test3\n\n");
    printf("x_ate([s2]P,[s1]Q)\n");
    BN12_X_ate_pairing(&test3,&s2P,&s1Q);
    printf("Miller's Algo. (X-ate) : %.2f[ms]\n",MILLER_XATE);
    printf("Final Exp. (easy)  : %.2f[ms]\n",FINALEXP_OPT_EASY);
    printf("Final Exp. (hard)  : %.2f[ms]\n",FINALEXP_OPT_HARD);
    printf("Final Exp. (total) : %.2f[ms]\n",MILLER_XATE+FINALEXP_OPT_EASY+FINALEXP_OPT_HARD);
    Fp12_printf("",&test3);
    printf("\n\n");
    
    printf("bilinear test\n");
    if(Fp12_cmp(&test1,&test2)==0 && Fp12_cmp(&test1,&test3)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
    
    mpz_clear(s12);
    mpz_clear(s1);
    mpz_clear(s2);
}

/*----------------------------------------------------------------------------*/
//BN12_SCM
int BN12_test_G1_SCM(int scm){
    int i,n=0;
    float scm_time=0,scm_2split_time=0,scm_2split_JSF_time=0,scm_lazy_time=0,scm_jacobian_time=0,scm_jacobian_table_time=0;
    struct timeval tv_A,tv_B;
    printf("================================================================================\n");
    printf("G1 SCM test\n\n");
    EFp12 A_EFp12,test1,test2,test3,test4,test5,test6;
    EFp12_init(&A_EFp12);
    EFp12_init(&test1);
    EFp12_init(&test2);
    EFp12_init(&test3);
    EFp12_init(&test4);
    EFp12_init(&test5);
    EFp12_init(&test6);
    mpz_t scalar;
    mpz_init(scalar);
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));

for(i=0;i<scm;i++){
    
    mpz_urandomm(scalar,state,order_z);
    BN12_EFp12_generate_G1(&A_EFp12);

    gettimeofday(&tv_A,NULL);
    EFp12_G1_SCM_plain(&test1,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_EFp12_G1_SCM_2split(&test2,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_2split_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_EFp12_G1_SCM_2split_JSF(&test3,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_2split_JSF_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_EFp12_G1_SCM_2split_JSF_lazy(&test4,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_lazy_time+=timedifference_msec(tv_A,tv_B);

    if(EFp12_cmp(&test1,&test2)!=0 || EFp12_cmp(&test1,&test3)!=0 || EFp12_cmp(&test1,&test4)!=0){
        printf("failed!\n\n");
	EFp12_printf("test1=",&test1);
	EFp12_printf("\ntest2=",&test2);
	EFp12_printf("\ntest3=",&test3);
	EFp12_printf("\ntest4=",&test4);
	printf("\n\n");
	return 1;
    }
}
    printf("BN12 G1 SCM.                           : %.4f[ms]\n",scm_time/scm);
    printf("BN12 G1 SCM 2split.                    : %.4f[ms]\n",scm_2split_time/scm);
    printf("BN12 G1 SCM 2split JSF.                : %.4f[ms]\n",scm_2split_JSF_time/scm);
    printf("BN12 G1 SCM 2split JSF lazy.           : %.4f[ms]\n",scm_lazy_time/scm);

    mpz_clear(scalar);

    return 0;
}

int BN12_test_G2_SCM(int scm){
    int i,n=0;
    float scm_time=0,scm_2split_time=0,scm_2split_JSF_time=0,scm_4split_time=0,scm_lazy_time=0,scm_jacobian_time=0,scm_jacobian_table_time=0;
    struct timeval tv_A,tv_B;
    printf("================================================================================\n");
    printf("G2 SCM\n\n");
    EFp12 A_EFp12,test1,test2,test3,test4,test5,test6,test7;
    EFp12_init(&A_EFp12);
    EFp12_init(&test1);
    EFp12_init(&test2);
    EFp12_init(&test3);
    EFp12_init(&test4);
    EFp12_init(&test5);
    EFp12_init(&test6);
    EFp12_init(&test7);
    mpz_t scalar;
    mpz_init(scalar);
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));

for(i=0;i<scm;i++){

    mpz_urandomm(scalar,state,order_z);
    //mpz_tdiv_q_2exp(scalar,scalar,30);//relic
    EFp12_generate_G2(&A_EFp12);

    gettimeofday(&tv_A,NULL);
    EFp12_G2_SCM_plain(&test1,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    EFp12_G2_SCM_2split(&test2,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_2split_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    EFp12_G2_SCM_2split_JSF(&test3,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_2split_JSF_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_EFp12_G2_SCM_4split(&test4,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_4split_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_EFp12_G2_SCM_4split_lazy(&test5,&A_EFp12,scalar);
    gettimeofday(&tv_B,NULL);
    scm_lazy_time+=timedifference_msec(tv_A,tv_B);


    if(EFp12_cmp(&test1,&test2)!=0 || EFp12_cmp(&test1,&test3)!=0 || EFp12_cmp(&test1,&test4)!=0 || EFp12_cmp(&test1,&test5)!=0){
        printf("failed!\n\n");
	EFp12_printf("test1=",&test1);
	EFp12_printf("\ntest2=",&test2);
	EFp12_printf("\ntest3=",&test3);
	EFp12_printf("\ntest4=",&test4);
	EFp12_printf("\ntest5=",&test5);
	printf("\n\n");
	return 1;
    }
}
    printf("BN12 G2 SCM.                           : %.4f[ms]\n",scm_time/scm);
    printf("BN12 G2 SCM 2split.                    : %.4f[ms]\n",scm_2split_time/scm);
    printf("BN12 G2 SCM 2split JSF.                : %.4f[ms]\n",scm_2split_JSF_time/scm);
    printf("BN12 G2 SCM 4split.                    : %.4f[ms]\n",scm_4split_time/scm);
    printf("BN12 G2 SCM 4split lazy.               : %.4f[ms]\n",scm_lazy_time/scm);
    mpz_clear(scalar);

    return 0;
}



int BN12_test_G3_EXP(int exp){
    int i,n=0;
    float exp_time=0,exp_2split_time=0,exp_2split_JSF_time=0,exp_4split_time=0,exp_lazy_time=0,exp_gs_time=0,exp_gs_lazy_time=0;
    struct timeval tv_A,tv_B;
    printf("================================================================================\n");
    printf("G3 Exp.\n\n");
    EFp12 P,Q;
    Fp12 A_Fp12,test1,test2,test3,test4,test5,test6,test7;
    EFp12_init(&P);
    EFp12_init(&Q);
    Fp12_init(&A_Fp12);
    Fp12_init(&test1);
    Fp12_init(&test2);
    Fp12_init(&test3);
    Fp12_init(&test4);
    Fp12_init(&test5);
    Fp12_init(&test6);
    Fp12_init(&test7);
    mpz_t scalar;
    mpz_init(scalar);
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));
    

for(i=0;i<exp;i++){

    mpz_urandomm(scalar,state,order_z);
    BN12_EFp12_generate_G1(&P);
    EFp12_generate_G2(&Q);
    BN12_Opt_ate_pairing(&A_Fp12,&P,&Q);

    
    gettimeofday(&tv_A,NULL);
    Fp12_G3_EXP_plain(&test1,&A_Fp12,scalar);
    gettimeofday(&tv_B,NULL);
    exp_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    Fp12_G3_EXP_2split(&test2,&A_Fp12,scalar);
    gettimeofday(&tv_B,NULL);
    exp_2split_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    Fp12_G3_EXP_2split_JSF(&test3,&A_Fp12,scalar);
    gettimeofday(&tv_B,NULL);
    exp_2split_JSF_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    BN12_Fp12_G3_EXP_4split(&test4,&A_Fp12,scalar);
    gettimeofday(&tv_B,NULL);
    exp_4split_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BN12_Fp12_G3_EXP_4split_lazy(&test5,&A_Fp12,scalar);
    gettimeofday(&tv_B,NULL);
    exp_lazy_time+=timedifference_msec(tv_A,tv_B);
    
    if(Fp12_cmp(&test1,&test2)!=0 || Fp12_cmp(&test1,&test3)!=0 || Fp12_cmp(&test1,&test4)!=0 || Fp12_cmp(&test1,&test5)!=0){
        printf("failed!\n\n");
	Fp12_printf("test1=",&test1);
	Fp12_printf("\ntest2=",&test2);
	Fp12_printf("\ntest3=",&test3);
	Fp12_printf("\ntest4=",&test4);
	Fp12_printf("\ntest5=",&test5);
	printf("\n\n");
	return 1;
    }
}
    printf("BN12 G3 exp.                           : %.4f[ms]\n",exp_time/exp);
    printf("BN12 G3 exp 2split.                    : %.4f[ms]\n",exp_2split_time/exp);
    printf("BN12 G3 exp 2split JSF.                : %.4f[ms]\n",exp_2split_JSF_time/exp);
    printf("BN12 G3 exp 4split.                    : %.4f[ms]\n",exp_4split_time/exp);
    printf("BN12 G3 exp 4split lazy.               : %.4f[ms]\n",exp_lazy_time/exp);

    mpz_clear(scalar);

    return 0;
}
