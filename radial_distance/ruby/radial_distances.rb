module Collection
  module Serial
    class Distances
      def initialize(references, targets)
        @references = references
        @targets = targets
      end

      def values
        size = [references.size, targets.size].min()
        (1..size).map do |index|
          references[index].distance(targets[i])
        end
      end
    end
  end

  module Multithread
  end
end
