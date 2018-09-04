class PolarPoint
  attr_reader :magnitude, :direction
  def initialize(magnitude, direction)
    @magnitude = magnitude
    @direction = direction
  end

  include Math
  def distance(target)
    return sqrt(
      (magnitude ** 2) + (target.magnitude ** 2) -
      2 * (magnitude * target.magnitude * cos(direction - target.direction))
    )
  end
end

module Collection
  class PolarPoints
    attr_reader :values

    def initialize(size, factory)
      @values = (1..size).map do
        factory.polar_point()
      end
    end
  end
end

module Factory
  class RandomPoint
    def initialize(max=(1 << 32))
      @max = max
    end

    include Math
    def polar_point
      PolarPoint.new(rand(@max), PI * rand(360) / 180)
    end
  end
end
