(ns counter
  (:use [twitter.oauth]
        [twitter.callbacks]
        [twitter.callbacks.handlers]
        [twitter.api.streaming])
  (:require [clojure.data.json :as json]
            [clojure.tools.logging :as log]
            [xively :as xiv])
  (:import [twitter.callbacks.protocols AsyncStreamingCallback]))

(def my-creds (make-oauth-creds ""         ;; *app-consumer-key*
                                ""         ;; *app-consumer-secret*
                                ""         ;; *user-access-token*
                                ""         ;; *user-access-token-secret*
                                ))

(def plus1tweet "england")
(def minus1tweet "germany")

(def rate 1000)

;; ====================================================

(def state (atom {}))

;; ====================================================

(def ctr-keys {plus1tweet :plus
               minus1tweet :minus})

(defn get-count [st]
  (-> (- (get st :plus 0)
         (get st :minus 0))
      (min 110)
      (max 0)))

(defn log-count [] (log/debug "COUNT" (get-count @state)))
(defn cancel-stream [st] ((:cancel (meta st))))
(defn cancelled? [st] ((:cancelled? (meta st))))

;; ====================================================

(defn- find-and-inc! [k tweet]
  (when (re-find (re-pattern k) (.toLowerCase tweet))
    (swap! state update-in [(ctr-keys k)] (fnil inc 0))))

(def ^:private streaming-callback
  (AsyncStreamingCallback.
   (fn [resp bs]
     (try
       (let [tweet (-> bs str json/read-json :text)]
         (find-and-inc! plus1tweet tweet)
         (find-and-inc! minus1tweet tweet)
         (log/info tweet)
         (log-count))
       (catch Exception e
         (log/error "parsing JSON" resp)
         (log-count)))
     (let [last (get @state :last 0)
           now (System/currentTimeMillis)]
       (when (> (- now last) rate)
         (try
           (log/info "sending to Xively")
           (xiv/send-to-xiv (get-count @state))
           (swap! state assoc :last now)
           (catch Exception e
             (log/error "sending to Xively" e))))))
   #(log/error (response-return-everything))
   exception-print))

(defn- start-stream []
  (swap! state assoc :stream
         (statuses-filter :params {:track (str plus1tweet "," minus1tweet)}
                          :oauth-creds my-creds
                          :callbacks streaming-callback)))

(defn stop-stream-watchdog []
  (when-let [dog (:stream-dog @state)]
    (dog)))

(defn start-stream-watchdog []
  (stop-stream-watchdog)
  (let [running (atom true)]
    (future
      (while @running
        (when (or (nil? (:stream @state)) (cancelled? (:stream @state)))
          (start-stream))
        (Thread/sleep 1000)))
    (swap! state assoc :stream-dog
           (fn []
             (reset! running false)
             (cancel-stream (:stream @state))))))

(defn reset-state! []
  (stop-stream-watchdog)
  (reset! state {})
  (start-stream-watchdog))

(defn reset-counters! []
  (swap! state assoc
         :plus 50
         :minus 0))

;; ====================================================

(comment

  (keys @state)

  [(:plus @state) (:minus @state)]

  (cancelled? (:stream @state))

  (start-stream-watchdog)

  (reset-counters!)

  ((:stream-dog @state))

)
