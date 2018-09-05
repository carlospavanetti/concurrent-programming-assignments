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
  end
end
