#ifndef oppwm_H_
#define oppwm_H_

extern void timer_init(void);
extern void TA2pwm_ctl(unsigned int R2,unsigned int R1);
extern void TA0pwm_ctl(unsigned int R4,unsigned int R3);
extern void fillup(void);
extern void filldown(void);
extern void upctl(float perc);
extern void downctl(float perc);
extern void stop(void);
extern void rotateL(void);
extern void rotateR(void);

#endif /* oppwm_H_ */
