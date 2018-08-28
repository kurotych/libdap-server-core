#include "dap_traffic_track_test.h"
#include <ev.h>

static struct ev_loop *loop;
#define LOG_TAG "DTTT"

static void success_callback() {
   // this causes the innermost ev_run to stop iterating
   dap_pass_msg("Call callback function");
   ev_break (EV_A_ EVBREAK_ONE);
}

static void error_callback() {
    log_it(L_ERROR, "ERROR CB");
    dap_fail("Error callback call, success_callback has no been called");
}

static void test_callback() {
    time_t timeout_sucess = 1;
    loop = EV_DEFAULT;
    dap_traffic_track_init(NULL, 1/* timeout sucess_callback*/);
    dap_traffic_set_callback(success_callback);

    /* Add error watcher*/
    static ev_timer timeout_error_watcher;
    ev_init(&timeout_error_watcher, error_callback);
    ev_timer_init (&timeout_error_watcher, error_callback, timeout_sucess * 2, 0.);
    ev_timer_start (loop, &timeout_error_watcher);
    ev_run (loop, 0);
}


void dap_traffic_track_tests_run(void) {
    dap_print_module_name("traffic_track");
    test_callback();
}