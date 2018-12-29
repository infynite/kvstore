//
// Created by Amin  Rezaei on 12/25/18.
//

#ifndef KVSTORE_MEM_H
#define KVSTORE_MEM_H

#include <mach/mach.h>
void load_mem(struct task_basic_info* t_info){
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)t_info,
                                  &t_info_count))
    {
    }
}
#endif //KVSTORE_MEM_H
