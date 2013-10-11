(ns site
  (:use [counter]
        [xively])
  (:require [compojure.core :refer (defroutes GET)]
            [compojure.handler :refer (api)]
            [ring.util.response :refer (redirect)]
            [hiccup.core :refer (html)]))

(defn- gen-page []
  (html
   [:h1 "Twitter counter 2000"]
   [:h2 (str "current count: " (get-count @state))]
   [:h3 "Keys"]
   (str ctr-keys)
   [:h3 "Counters"]
   (list "plus:" (:plus @state) " -- minus:" (:minus @state))
   [:h3 "stream cancelled?"]
   (try (cancelled? (:stream @state)) (catch Exception e "key missing"))
   [:h3 "Control me"]
   [:a {:href "/reset"} "Reset everything"]
   [:br]
   [:a {:href "/stop"} "Stop watchdog"]
   [:br] [:br]
   [:a {:href "/resetcounters50"} "Reset counters 50-50"]
   [:br]
   [:a {:href "/resetcounters00"} "Reset counters 0-0"]
   [:br] [:br]
   [:a {:href "/send"} "Send counters to Xively"]))

(defroutes routes
  (GET "/" [] (gen-page))
  (GET "/reset" [] (do (reset-state!) (redirect "/")))
  (GET "/resetcounters50" [] (do (reset-counters! 50 0) (redirect "/")))
  (GET "/resetcounters00" [] (do (reset-counters! 0 0) (redirect "/")))
  (GET "/send" [] (do (send-to-xiv (get-count @state)) (redirect "/")))
  (GET "/stop" [] (do (stop-stream-watchdog) (redirect "/"))))

(def the-site (api routes))
