# Copyright 2007-2010 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

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
