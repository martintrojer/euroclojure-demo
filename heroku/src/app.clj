(ns app
  (:use [site]
        [counter])
  (:require [ring.adapter.jetty :as jetty]))

(defn -main [port]
  (jetty/run-jetty the-site {:port (Integer. port) :join? false})
  (start-stream-watchdog))


;; git push heroku master
;; (heroku ps:scale web=1)
;; heroku ps
;; heroku open
;; heroku logs
