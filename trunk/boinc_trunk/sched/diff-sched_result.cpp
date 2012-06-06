Index: sched_result.cpp
===================================================================
--- sched_result.cpp    (Revision 19193)
+++ sched_result.cpp    (Arbeitskopie)
@@ -107,7 +107,6 @@
             );
 
             g_reply->result_acks.push_back(std::string(rp->name));
-            continue;
         }
 
         if (config.debug_handle_results) {
@@ -185,7 +184,7 @@
                 g_reply->host.id, srip->id, srip->workunitid, srip->server_state
             );
             srip->id = 0;
-            g_reply->result_acks.push_back(std::string(rp->name));
+//            g_reply->result_acks.push_back(std::string(rp->name));
             continue;
         }
 
@@ -196,7 +195,7 @@
                 time_to_string(srip->received_time)
             );
             srip->id = 0;
-            g_reply->result_acks.push_back(std::string(rp->name));
+//            g_reply->result_acks.push_back(std::string(rp->name));
             continue;
         }
 
