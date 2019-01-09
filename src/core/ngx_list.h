
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 *
 * ngx_list_t 是Nginx封装的链表容器，它在Nginx中使用的很频繁
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

/**
 * 结构设计的好处
 * 1. 链表中存储的元素是灵活的，它可以是任何一种数据结构
 * 2. 链表元素需要占用的内存由ngx_list_t管理，他已经通过数组分配好了
 * 3. 小块的内存使用链表访问效率是低下的，使用数组通过偏移量来直接访问内存则要高效的多
 */

typedef struct ngx_list_part_s  ngx_list_part_t;

struct ngx_list_part_s {
    void             *elts;  // 节点中存放具体元素的内存的开始地址。
    ngx_uint_t        nelts; // 节点中已有元素个数。这个值是不能大于链表头节点ngx_list_t类型中的nalloc字段的。
    ngx_list_part_t  *next;
};


typedef struct {
    ngx_list_part_t  *last; // 指向该链表的最后一个节点
    ngx_list_part_t   part; // 该链表的首个存放具体元素的节点
    size_t            size; // 数组存储的是某种类型的数据结构，且ngx_list_t是非常灵活的数据结构，只是通过size限制每一个数组元素占用空间的大小
    ngx_uint_t        nalloc; // 每个节点所含的固定大小的数组的容量。
    ngx_pool_t       *pool; // 该list使用的分配内存的pool。
} ngx_list_t;


// ngx_list_create 创建元素时，pool参数是内存池对象 size是每个元素的大小 n是每个链表可容纳元素的个数
ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    //内存的开始地址
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part; // part用于指向链表中的每一个ngx_list_part_t数组
 *  data = part->elts; // 根据链表中的数据类型，把数组里的elts转化为该类型使用
 *
 *  // i表示元素在链表的每个ngx_list_part_t数组里的序号
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) { // 地址
 *          if (part->next == NULL) {
 *              //如果某个ngx_list_part_t数组的next指针为空，则说明遍历完链表
 *              break;
 *          }
 *
 *          // 访问下一个ngx_list_part_t
 *          part = part->next;
 *          data = part->elts;
 *
 *          将序号置为0，准备重新访问下一个数组
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


//调用ngx_list_push表示添加新的元素，传入的参数是ngx_list_push得到返回的元素地址，再对返回的地址进行赋值
void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
