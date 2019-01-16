
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


// 动态数组
typedef struct {
    void        *elts;   //elts指向数组的首地址
    ngx_uint_t   nelts;  //nelts数组中已经使用的元素个数
    size_t       size;   //每个数组元素占用的内存大小
    ngx_uint_t   nalloc; //当前数组中能够容纳元素的总大小
    ngx_pool_t  *pool;   //内存池对象
} ngx_array_t;


//创建新的动态数组
ngx_array_t *ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);
//销毁数组对象，内存被内存池回收
void ngx_array_destroy(ngx_array_t *a);
//添加一个数组元素
void *ngx_array_push(ngx_array_t *a);
//添加n个数组元素
void *ngx_array_push_n(ngx_array_t *a, ngx_uint_t n);


//初始化
static ngx_inline ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    //初始化数组成员，注意：nelts必须比elts先初始化
    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    //分配数组数据所需要的内存
    array->elts = ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return NGX_ERROR;
    }

    return NGX_OK;
}

#endif /* _NGX_ARRAY_H_INCLUDED_ */
