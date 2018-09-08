module Reductor
  module Serial
    class MaximumOf
      def initialize(collection)
        @collection = collection
      end

      def value()
        @collection.values().min
      end
    end
  end

  module Multithread
    class MaximumOf
      def initialize(collection, threads)
        @collection = collection
        @threads = threads
      end

      def value()
        values = @collection.values()
        candidates = Array.new(@threads, Float::INFINITY)
        @threads.times.map do |id|
          Thread.new do
            (id..values.size()).step(@threads) do |index|
              candidates[id] = [candidates[id], values[index]].min()
            end
          end
        end
        candidates.min
      end
    end
  end
end
