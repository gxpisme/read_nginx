
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * find the middle queue element if the queue has odd number of elements
 * or the first element of the queue's second part otherwise
 *
 * 找中间元素
 */

ngx_queue_t *
ngx_queue_middle(ngx_queue_t *queue)
{
    ngx_queue_t  *middle, *next;

    //链表的头元素
    middle = ngx_queue_head(queue);

    //链表的头元素与尾元素相等，则就一个元素，返回该元素即可
    if (middle == ngx_queue_last(queue)) {
        return middle;
    }

    //这里也是头元素
    next = ngx_queue_head(queue);

    //开始循环
    //1.采用了每循环一次，middle向后移一个指针
    //2.每循环一次，next向后移动两个指针
    //3.middle走一步，next走两步，当next走到尾元素，则正好输出middle，完美
    for ( ;; ) {
        //从头元素开始，middle依次向后，每循环一次，向后移一个指针
        middle = ngx_queue_next(middle);

        //也是从头元素开始，next，每循环一次，向后移动两个指针
        //next每移动一次之后，要判读next与最后一个元素是否相等
        next = ngx_queue_next(next);
        if (next == ngx_queue_last(queue)) {
            return middle;
        }

        next = ngx_queue_next(next);
        if (next == ngx_queue_last(queue)) {
            return middle;
        }
    }
}


/* the stable insertion sort */

/**
 * 无须的双向链表，排序成有序的双向链表 很好玩，很精妙的逻辑
 */
void
ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *))
{
    ngx_queue_t  *q, *prev, *next;

    //链表的头元素
    q = ngx_queue_head(queue);

    //头元素等于尾元素
    if (q == ngx_queue_last(queue)) {
        return;
    }

    //从头元素向后循环
    for (q = ngx_queue_next(q); q != ngx_queue_sentinel(queue); q = next) {

        //向前遍历
        prev = ngx_queue_prev(q);
        //向后遍历
        next = ngx_queue_next(q);

        //移除q元素
        ngx_queue_remove(q);

        //该循环保证了，q大于prev的最大值，一定q放在了最大值prev的后面
        do {
            //当q大于prev时，退出循环
            if (cmp(prev, q) <= 0) {
                break;
            }

            prev = ngx_queue_prev(prev);

        } while (prev != ngx_queue_sentinel(queue));

        //将q元素插入在prev的后面，保证q元素大于prev
        ngx_queue_insert_after(prev, q);
    }
}
