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
require 'rubygems'
require 'spec/rake/spectask'
require 'spec/rake/verify_rcov'
require File.expand_path('lib/mkdtemp/version.rb', File.dirname(__FILE__))

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

file 'ext/Makefile' => ['ext/depend', 'ext/extconf.rb'] do
  Dir.chdir 'ext' do
    ruby './extconf.rb'
  end
end

desc 'Build extension'
task :make => ['ext/mkdtemp.c', 'ext/ruby_compat.h', 'ext/Makefile'] do |t|
  Dir.chdir 'ext' do
    system 'make'
  end
end

desc 'Buils the YARD HTML files'
task :yard do
  sh 'yardoc -o html --title mkdtemp - doc/README'
end

desc 'Upload YARD HTML'
task :upload_yard => :yard do
  require 'yaml'
  config = YAML.load_file('.config.yml')
  raise ':yardoc_host not configured' unless config.has_key?(:yardoc_host)
  raise ':yardoc_path not configured' unless config.has_key?(:yardoc_path)
  sh "scp -r html/* #{config[:yardoc_host]}:#{config[:yardoc_path]}"
end

EXT_FILE = "ext/mkdtemp.#{Config::CONFIG['DLEXT']}"

file EXT_FILE => :make

GEM_FILE_DEPENDENCIES = [
  EXT_FILE, # not actually included in gem, but we want to be sure it builds
  'ext/depend',
  'ext/extconf.rb',
  'ext/mkdtemp.c',
  'ext/ruby_compat.h',
  'lib/mkdtemp/version.rb',
  'mkdtemp.gemspec'
]

GEM_FILE = "mkdtemp-#{Dir::Mkdtemp::VERSION}.gem"

file GEM_FILE => GEM_FILE_DEPENDENCIES do
  system 'gem build mkdtemp.gemspec'
end

desc 'Build gem ("gem build")'
task :build => GEM_FILE

desc 'Publish gem ("gem push")'
task :push => :build do
  system "gem push #{GEM_FILE}"
end
