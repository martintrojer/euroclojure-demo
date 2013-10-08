(ns site
  (:use [counter])
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
   [:h3 "Reset me"]
   [:a {:href "/reset"} "Reset it all"]
   [:br] [:br]
   [:a {:href "/resetcounters"} "Reset counters 50-50"]
   [:br] [:br]

   [:a {:href "/stop"} "Stop watchdog"]))

(defroutes routes
  (GET "/" [] (gen-page))
  (GET "/reset" []
       (do (reset-state!) (redirect "/")))
  (GET "/resetcounters" []
       (do (reset-counters!) (redirect "/")))
  (GET "/stop" []
       (do (stop-stream-watchdog) (redirect "/"))))

(def the-site (api routes))
