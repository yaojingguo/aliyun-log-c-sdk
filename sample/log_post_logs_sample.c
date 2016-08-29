#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "log_auth.h"
#include "log_util.h"
#include "log_api.h"
#include "log_config.h"

void log_post_logs_sample()
{
    aos_pool_t *p = NULL;
    aos_status_t *s = NULL;
    aos_pool_create(&p, NULL);
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "__source__", "127.0.0.1");
    cJSON_AddStringToObject(root, "__topic__", "topic");
    //cJSON *tags = cJSON_CreateObject();
    //cJSON_AddItemToObject(root, "__tags__", tags);
    //cJSON_AddStringToObject(tags, "tag1", "tag1");
    //cJSON_AddStringToObject(tags, "tag2", "tag2");
    cJSON *logs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "__logs__", logs);
    cJSON *log1 = cJSON_CreateObject();
    cJSON_AddStringToObject(log1, "level", "error1");
    cJSON_AddStringToObject(log1, "message", "c sdk test message1");
    cJSON_AddNumberToObject(log1, "__time__", apr_time_now()/1000000);
    cJSON *log2 = cJSON_CreateObject();
    cJSON_AddStringToObject(log2, "level", "error2");
    cJSON_AddStringToObject(log2, "message", "c sdk test message2");
    cJSON_AddNumberToObject(log2, "__time__", apr_time_now()/1000000);
    cJSON_AddItemToArray(logs, log1);
    cJSON_AddItemToArray(logs, log2);
    s = log_post_logs(p, LOG_ENDPOINT, ACCESS_KEY_ID, ACCESS_KEY_SECRET, PROJECT_NAME, LOGSTORE_NAME, root);
    if (aos_status_is_ok(s)) {
        printf("post logs succeeded\n");
    } else {
        printf("%d\n",s->code);
        printf("%s\n",s->error_code);
        printf("%s\n",s->error_msg);
        printf("%s\n",s->req_id);
        printf("put logs failed\n");
    }
    cJSON_Delete(root);
    aos_pool_destroy(p);
}

void log_post_logs_sample2(){
    aos_pool_t *p = NULL;
    aos_status_t *s = NULL;
    aos_pool_create(&p, NULL);
    
    while(1){
        
        log_group_builder* bder = log_group_create();
        add_source(bder,"mSource",sizeof("mSource"));
        add_topic(bder,"mTopic", sizeof("mTopic"));
        add_log(bder);
        add_log_key_value(bder, "K", sizeof("K"), "V", sizeof("V"));
        
        
        
        //log_http_cont* req =  log_get_http_cont(LOG_ENDPOINT, ACCESS_KEY_ID, ACCESS_KEY_SECRET,NULL, PROJECT_NAME, LOGSTORE_NAME, bder);
        
        //log_clean_http_cont(req);
        
        s = log_post_logs_from_proto_buf(LOG_ENDPOINT, ACCESS_KEY_ID, ACCESS_KEY_SECRET,NULL, PROJECT_NAME, LOGSTORE_NAME, bder);
        if (aos_status_is_ok(s)) {
            //printf("post logs succeeded\n");
        } else {
            printf("%d\n",s->code);
            printf("%s\n",s->error_code);
            printf("%s\n",s->error_msg);
            printf("%s\n",s->req_id);
            printf("put logs failed\n");
        }
        
        log_group_destroy(bder);
        
    }
}

int main(int argc, char *argv[])
{
    if (aos_http_io_initialize("linux-x86_64", 0) != AOSE_OK) {
        exit(1);
    }
    log_post_logs_sample2();
    aos_http_io_deinitialize();
    return 0;
}
