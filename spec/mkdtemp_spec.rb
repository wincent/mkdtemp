# Copyright 2008-2010 Wincent Colaiuta. All rights reserved.
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

require File.join(File.dirname(__FILE__), 'spec_helper')

describe 'Dir.mkdtemp' do
  before do
    if not File.exist? '/tmp'
      pending "cannot run spec because '/tmp' does not exist"
    elsif not File.directory? '/tmp'
      pending "cannot run spec because '/tmp' is not a directory"
    elsif not File.writable? '/tmp'
      pending "cannot run spec because '/tmp' is not writeable"
    end
  end

  it 'should return the modified template path' do
    path = Dir.mkdtemp
    path.should be_kind_of(String)
    path.should_not == '/tmp/temp.XXXXXX'
  end

  it 'should create the directory corresponding to the template' do
    path = Dir.mkdtemp
    File.exist?(path).should == true
    File.directory?(path).should == true
  end

  it 'should create the directory with write permissions' do
    path = Dir.mkdtemp
    File.writable?(path).should == true
  end

  it 'should create the directory with read permissions' do
    path = Dir.mkdtemp
    File.readable?(path).should == true
  end

  it 'should create the directory with secure ownership (same user as caller)' do
    path = Dir.mkdtemp
    File.owned?(path).should == true
  end

  it 'should complain if passed a template that includes non-existentent parent directories' do
    non_existent_dir = '/temp-which-does-not-exist'
    if File.exist? non_existent_dir
      pending "cannot run because '#{non_existent_dir}' exists"
    end
    lambda { Dir.mkdtemp "/#{non_existent_dir}/temp.XXXXXX" }.should raise_error(SystemCallError)
  end

  it 'should use "/tmp/temp.XXXXXX" as a template if passed nil' do
    path = Dir.mkdtemp
    path.should match(%r{\A/tmp/temp\..{6}\z})
    path.should_not == '/tmp/temp.XXXXXX'
  end

  it 'should use substitute random characters for the trailing "Xs" in the template' do
    path = Dir.mkdtemp '/tmp/test.XXXXXX'
    path.should_not == '/tmp/test.XXXXXX'
  end

  it 'should leave the prefix portion of the template unchanged' do
    path = Dir.mkdtemp '/tmp/test.XXXXXX'
    path.should match(%r{\A/tmp/test\..{6}\z})
  end
end
