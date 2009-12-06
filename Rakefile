# Copyright 2007-2008 Wincent Colaiuta
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require 'rubygems'
require 'spec/rake/spectask'
require 'spec/rake/verify_rcov'
require File.join(File.dirname(__FILE__), 'lib', 'mkdtemp', 'version.rb')

CLEAN.include   Rake::FileList['**/*.so', '**/*.bundle', '**/*.o', '**/mkmf.log', '**/Makefile']

task :default => :all

desc 'Build all and run all specs'
task :all => [:make, :spec]

desc 'Run specs with coverage'
Spec::Rake::SpecTask.new('coverage') do |t|
  t.spec_files  = FileList['spec/**/*_spec.rb']
  t.rcov        = true
  t.rcov_opts = ['--exclude', "spec"]
end

desc 'Run specs'
Spec::Rake::SpecTask.new('spec') do |t|
  t.spec_files  = FileList['spec/**/*_spec.rb']
end

desc 'Verify that test coverage is above minimum threshold'
RCov::VerifyTask.new(:verify => :spec) do |t|
  t.threshold   = 99.2 # never adjust expected coverage down, only up
  t.index_html  = 'coverage/index.html'
end

desc 'Generate specdocs for inclusions in RDoc'
Spec::Rake::SpecTask.new('specdoc') do |t|
  t.spec_files  = FileList['spec/**/*_spec.rb']
  t.spec_opts   = ['--format', 'rdoc']
  t.out         = 'specdoc.rd'
end

desc 'Build extension'
task :make do |t|
  system %{cd ext && ruby ./extconf.rb && make && cd -}
end

Rake::RDocTask.new do |t|
  t.rdoc_files.include 'doc/README', 'ext/mkdtemp.c'
  t.options           << '--charset' << 'UTF-8' << '--inline-source'
  t.main              = 'doc/README'
  t.title             = 'mkdtemp documentation'
end

desc 'Upload RDoc to RubyForge website'
task :upload_rdoc => :rdoc do
  sh 'scp -r html/* rubyforge.org:/var/www/gforge-projects/mkdtemp/'
end


SPEC = Gem::Specification.new do |s|
  s.name              = 'mkdtemp'
  s.version           = Dir::Mkdtemp::VERSION
  s.author            = 'Wincent Colaiuta'
  s.email             = 'win@wincent.com'
  s.homepage          = 'http://mkdtemp.rubyforge.org/'
  s.rubyforge_project = 'mkdtemp'
  s.platform          = Gem::Platform::RUBY
  s.summary           = 'Secure creation of temporary directories'
  s.description       = <<-ENDDESC
    mkdtemp is a C extension that wraps the Standard C Library function
    of the same name to make secure creation of temporary directories
    easily available from within Ruby.
  ENDDESC
  s.require_paths     = ['ext', 'lib']
  s.has_rdoc          = true

  # TODO: add 'docs' subdirectory, 'README.txt' when they're done
  s.files             = FileList['{lib,spec}/**/*', 'ext/*.{c,h,rb}', 'ext/depend'].to_a
  s.extensions        = ['ext/extconf.rb']
end

task :package => [:clobber, :all, :gem]
Rake::GemPackageTask.new(SPEC) do |t|
  t.need_tar      = true
end
