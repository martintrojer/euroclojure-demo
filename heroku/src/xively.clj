(ns xively
  (:require [clj-http.client :as client]
            [clojure.data.json :as json]))

(def xiv-key "zng8xuV8tSYuBpecPv11tWzSQMWl7lCl6Opmdx3hQJWHX4wU")
(def xiv-feed 526645367)

(defn send-to-xiv [val]
  (client/put (format "http://api.xively.com/v2/feeds/%s.json" xiv-feed)
              {:headers {"X-APIKEY" xiv-key}
               :body (json/write-str
                      {:version "1.0.0"
                       :datastreams [{:id "score"
                                      :current_value val}]})}))
