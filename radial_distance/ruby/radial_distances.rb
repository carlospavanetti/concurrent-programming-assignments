module Collection
  module Serial
    class Distances
      def initialize(references, targets)
        @references = references
        @targets = targets
      end

      def values
        references, targets = @references.values(), @targets.values()
        size = [references.size, targets.size].min()
        (0..size-1).map do |index|
          ans = references[index].distance(targets[index])
          if block_given?
            yield ans
          else
            ans
          end
        end
      end
    end
  end

  module Multithread
  end
end
