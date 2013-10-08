(defproject twitter-counter "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [[org.clojure/clojure "1.5.1"]

                 [twitter-api "0.7.4"]

                 [ring/ring-jetty-adapter "1.2.0"]
                 [compojure "1.1.5"]
                 [hiccup "1.0.4"]

                 [clj-http "0.7.7"]
                 [org.clojure/data.json "0.2.3"]

                 [org.clojure/tools.logging "0.2.6"]
                 [org.slf4j/slf4j-simple "1.7.5"]]
  :uberjar-name "twitter-counter-standalone.jar"
  :min-lein-version "2.0.0")
