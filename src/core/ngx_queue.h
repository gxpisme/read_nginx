
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_


/**
 * 队列结构，其实质是具有头节点的双向循环链表
 */
typedef struct ngx_queue_s  ngx_queue_t;

/**
 * 队列中每个节点结构，只有两个指针，并没有数据区
 */
struct ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};


/**
 * q为链表容器结构体ngx_queue_t的指针
 * 链表容器q初始化，会设置为空的链表
 */
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


/**
 * 检测链表容器是否为空
 */
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)


/**
 * 将元素x，插入h链表的头部
 *
 * 图解
 * http://cdn.xpisme.com/201901151909_496.png
 */
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define ngx_queue_insert_after   ngx_queue_insert_head


/**
 * 将元素x，插入h链表的尾部
 *
 * 图解
 * http://cdn.xpisme.com/201901152024_507.png
 */
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


/**
 * 链表的头元素
 */
#define ngx_queue_head(h)                                                     \
    (h)->next

/**
 * 链表的尾元素
 */
#define ngx_queue_last(h)                                                     \
    (h)->prev


#define ngx_queue_sentinel(h)                                                 \
    (h)

/**
 * 链表的后一个元素
 */
#define ngx_queue_next(q)                                                     \
    (q)->next

/**
 * 链表的前一个元素
 */
#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if (NGX_DEBUG)

/**
 * 在链表中移除x元素
 */
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


/**
 * h为链表容器结构体ngx_queue_t的指针 该函数用于拆分链表
 * h是链表容器，而q是链表h中的一个元素。
 * 将链表h以元素q为界拆分成两个链表 h 和 n
 *
 * 图解
 * http://cdn.xpisme.com/201901152056_612.png
 */
#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;

/**
 * h为链表容器结构体ngx_queue_t的指针，n为另一个链表容器结构体ngx_queue_t的指针
 * 合并链表，将n链表添加到h链表的末尾
 *
 * 图解
 * http://cdn.xpisme.com/201901161012_123.png
 */
#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
