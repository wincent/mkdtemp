# Copyright 2008 Wincent Colaiuta
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
