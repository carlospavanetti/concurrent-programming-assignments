class RadialDistancesArguments
  def initialize(arguments)
    @arguments = arguments
  end

  def size()
    value('size').to_i
  end

  def threads()
    value('threads').to_i
  end

  def maximum()
    argument('maximum') == '--maximum'
  end

  private
  def value(target)
    argument(target)&.slice(target.size() + 3 .. -1).to_i
  end

  def argument(target)
    @arguments.select do |arg|
      arg.start_with?("--#{target}")
    end.last()
  end
end
