#ifndef __VECTOR_TESTS
#define __VECTOR_TESTS
#include <test_defs.h>
#include <ca_vector.h>

#include <munit.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>

MunitResult
test_vector_new(const MunitParameter params[],
                void *user_data_or_fixture),
test_vector_read(const MunitParameter params[],
                 void *user_data_or_fixture);

MunitTest vector_tests [] = {
      {"vector_new", test_vector_new,
          NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
      {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

#endif
#if defined(CA_VEC_TEST) | defined(DEBUG) | defined(TEST)

MunitResult test_vector_new(const MunitParameter params[],
                            void *user_data_or_fixture) {

  errno=0;
  ca_vector *vec=ca_vector_new(CA_NO_DATA);
  if(vec==NULL){
        fprintf(stderr,
                "Could not create vector: %s",
                strerror(errno));
        fail();
  }

  if(vec->len.mem != CA_DEF_LEN
     || vec->len.cap != vec->len.mem){
      munit_logf(MUNIT_LOG_ERROR,
                "Wrong length %s",
                strerror(errno));
      fail();
  }

  if(!ca_vector_del(&vec)){
        munit_logf(MUNIT_LOG_ERROR,
                "Could not delete vector: %s",
                strerror(errno));
        fail();
  }

  if(vec != NULL){
      munit_logf(MUNIT_LOG_ERROR,
                "Wrong length (%lu %lu:%d) %s",
                vec->len.mem,vec->len.cap,CA_DEF_LEN,
                strerror(errno));
      fail();
  }

  if((vec=ca_vector_new(10))==NULL){
        fprintf(stderr,
                "Could not create vector: %s",
                strerror(errno));
        fail();
  }

  if(vec->len.mem != CA_DEF_LEN
     || vec->len.cap != vec->len.mem){
      munit_logf(MUNIT_LOG_ERROR,
                "Wrong length (%lu %lu:%d) %s",
                vec->len.mem,vec->len.cap,CA_DEF_LEN,
                strerror(errno));
      fail();
  }

  if(!ca_vector_del(&vec)){
        munit_logf(MUNIT_LOG_ERROR,
                "Could not delete vector: %s",
                strerror(errno));
        fail();
  }

  if((vec=ca_vector_new(128))==NULL){
        fprintf(stderr,
                "Could not create vector: %s",
                strerror(errno));
        fail();
  }

  if(vec->len.mem <= CA_DEF_LEN
     || vec->len.cap != vec->len.mem){
      munit_logf(MUNIT_LOG_ERROR,
                "Wrong length (%lu %lu:%d) %s",
                vec->len.mem,vec->len.cap,CA_DEF_LEN,
                strerror(errno));
      fail();
  }

  test_resolve();
}

MunitResult test_vector_write(const MunitParameter params[],
                         void *user_data_or_fixture){

  ca_vector *v=NULL;
  if(!(v=ca_vector_new(CA_NO_DATA))){
        fprintf(stderr,
                "Could not create vector: %s",
                strerror(errno));
        fail();
  }

  test_resolve();
}

#endif
