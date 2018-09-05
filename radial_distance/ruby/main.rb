require './arguments'
require './polar_point'
require './radial_distances.rb'
require './maximum_of.rb'

require 'benchmark'

arguments  = RadialDistancesArguments.new(ARGV)
generator  = Factory::RandomPoint.new(100)
references = Collection::PolarPoints.new(arguments.size(), generator)
targets    = Collection::PolarPoints.new(arguments.size(), generator)
distances  = Collection::Serial::Distances.new(references, targets)
maximum_of = Reductor::Serial::MaximumOf.new(distances)

if arguments.maximum()
  puts(Benchmark.measure {maximum_of.value()})
else
  puts(Benchmark.measure {distances.value()})
end
