
#include<stdio.h>
#pragma warning (disable:4996)

int add(int n1, int n2)
{
    int add;
    add = n1 + n2;
    return add;

}

int diff(int add_rsl, int diff2)
{
    int diff_rsl;
    diff_rsl = add_rsl - diff2;
    return diff_rsl;
}

int multi(int diff_rsl, int multi2)
{
    int multi_rsl;
    multi_rsl = diff_rsl - multi2;
    return multi_rsl;
}

int operate(int (*func)(int, int), int x, int y) 
{
    int func_rsl;
    func_rsl = func(x, y);
    return func_rsl;
}

int main(void) {

    //int p1 = 0 ,p2 =0;
    int add_rsl,add1, add2;
    int diff_rsl,diff;
    int multi_rsl,multi;

    puts("enter any numbers");

    
    //これだと読み取った数がadd1, add2に代入される
    /*
    int add1 = scanf("%d", &p1);
    int add2 = scanf("%d", &p2);
    */

    //正しくは
    printf("足し算用\n整数1:");  scanf("%d", &add1);
    printf("整数2:");  scanf("%d", &add2);
    printf("引き算用\n整数:"); scanf("%d", &diff);
    printf("掛け算用\n整数:"); scanf("%d", &multi);

    add_rsl = add(add1, add2);
    diff_rsl = operate(diff, add_rsl, diff);
    multi_rsl = operate(multi, diff_rsl, multi);
   
    //printf("add1:%d \n add2:%d \n", add1, add2);
    printf("足し算;%d\n引き算;%d\n掛け算;%d", add(add1, add2),diff_rsl,multi_rsl);

    


    return 0;
}


