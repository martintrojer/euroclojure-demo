(ns xively
  (:require [clj-http.client :as client]
            [clojure.data.json :as json]))

(def xiv-key "")
(def xiv-feed -1)

(defn send-to-xiv [val]
  (client/put (format "http://api.xively.com/v2/feeds/%s.json" xiv-feed)
              {:headers {"X-APIKEY" xiv-key}
               :body (json/write-str
                      {:version "1.0.0"
                       :datastreams [{:id "score"
                                      :current_value val}]})}))
