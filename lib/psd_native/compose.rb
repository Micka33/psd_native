require 'psd'
require 'psd_native/psd_native'

class PSD
  module Compose
    PSDNative::Compose.methods(false).each do |meth|
      define_method(meth) do |*args|
        do_blend meth, *args
      end
    end

    private

    DEFAULT_OPTS = {
      opacity: 255,
      fill_opacity: 255
    }

    def do_blend(blend, fg, bg, opts={})
      File.open('/Users/cassymickael/Desktop/micka.tmp.text', 'a') { |file| file.write("blend:#{blend.to_s}, opts:#{opts.to_s}\n") }
      DEFAULT_OPTS[:opacity] = opts.to_i if opts.is_a? Integer 
      opts = DEFAULT_OPTS.merge(opts) unless opts.is_a? Integer 
      PSDNative::Compose.send(blend, fg, bg, opts)
    end
  end
end
