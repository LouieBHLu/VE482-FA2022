//
// Created by citrate on 11/23/21.
//

#ifndef DICE_H
#define DICE_H

#define DICE_DEVS 3  /* Number of dice devices */
#define DICE_NUM 2   /* Initial number of dice in a device */
#define GEN_SIDES 20 /* default number of sides of `arbitrary` dice game */

struct dice_dev{
    int num;               /* number of dice in this device */
    int dice_type;	       /* type of dice game:  regular; backgammon; generic */
    struct cdev dice_cdev; /* cdev object of dice */
};

#endif //DICE_H
