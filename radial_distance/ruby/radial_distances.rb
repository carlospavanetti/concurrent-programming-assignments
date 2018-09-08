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
    class Distances
      def initialize(references, targets, threads)
        @references = references
        @targets = targets
        @threads = threads
      end

      def values
        references, targets = @references.values(), @targets.values()
        size = [references.size, targets.size].min()
        @values = Array.new(size)
        @threads.times.map do |id|
          Thread.new do
            (id..size-1).step(@threads).each do |index|
              @values[index] = references[index].distance(targets[index])
            end
          end
        end
        return @values
      end
    end
  end
end
