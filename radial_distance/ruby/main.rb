require './arguments'
require './polar_point'
require './radial_distances.rb'

arguments  = RadialDistancesArguments.new(ARGV)
generator  = Factory::RandomPoint.new(100)
references = Collection::PolarPoints.new(arguments.size(), generator)
targets    = Collection::PolarPoints.new(arguments.size(), generator)
distances  = Collection::Serial::Distances.new(references, targets)
