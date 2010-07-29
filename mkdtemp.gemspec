require File.expand_path('lib/mkdtemp/version.rb', File.dirname(__FILE__))

Gem::Specification.new do |s|
  s.name              = 'mkdtemp'
  s.version           = Dir::Mkdtemp::VERSION
  s.author            = 'Wincent Colaiuta'
  s.email             = 'win@wincent.com'
  s.homepage          = 'https://wincent.com/products/mkdtemp'
  s.rubyforge_project = 'mkdtemp'
  s.platform          = Gem::Platform::RUBY
  s.summary           = 'Secure creation of temporary directories'
  s.description       = <<-ENDDESC
    mkdtemp is a C extension that wraps the Standard C Library function
    of the same name to make secure creation of temporary directories
    easily available from within Ruby.
  ENDDESC
  s.require_paths     = ['ext', 'lib']
  s.has_rdoc          = false

  # TODO: add 'docs' subdirectory, 'README.txt' when they're done
  s.files             = Dir['lib/**/*', 'ext/*.{c,h,rb}', 'ext/depend']
  s.extensions        = ['ext/extconf.rb']

  s.add_development_dependency 'rspec', '>= 2.0.0.beta'
end
