require './arguments'
require './polar_point'

arguments  = RadialDistancesArguments.new(ARGV)
generator  = Factory::RandomPoint.new(100)
references = Collection::PolarPoints.new(arguments.size(), generator)
targets    = Collection::PolarPoints.new(arguments.size(), generator)
