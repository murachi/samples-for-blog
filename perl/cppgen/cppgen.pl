#!/usr/bin/perl
# cppgen.pl - C++ クラス定義ファイル (.h, .cpp) のひな形を作成する。
use strict;
use warnings;

use Getopt::Compact;

my $get_options = Getopt::Compact->new(
	name => 'C++ File Template Generator',
	version => '0.1.0',
	modes => [qw(pimpl interface virtual noncopyable)],
	struct => [
		[[qw(c classname)], 'Specify a name of class', '=s'],
		[[qw(d namespace domain)], 'Specify a name of namespace (domain)', '=s'],
		[[qw(s summary)], 'Write summary of class', ':s'],
		[[qw(e extends)], 'List of extended classes', ':s'],
		[[qw(C copy)], 'Generate copy constructor placement'],
		[[qw(M move)], 'Generate move constructor placement'],
	],
	configure => { posix_default => 1, ignore_case => 0, gnu_compat => 1, bundling => 1 },
);
my $opts = $get_options->opts;

# class と namespace は必須
if (!defined $opts->{classname} || !defined $opts->{namespace}) {
	print "$0: Need classname (-c) and namespace (-d) specifying.\n";
	print $get_options->usage;
	exit 1;
}

# interface モードと pimpl モードは混在不可
if ($opts->{interface} && $opts->{pimpl}) {
	print "$0: Can't select both modes of -i and -p.\n";
	print $get_options->usage;
	exit 1;
}

# noncopyable モードを使用する場合、コピーコンストラクタ、ムーブコンストラクタは実装不能
if ($opts->{noncopyable} && ($opts->{copy} || $opts->{move})) {
	print "$0: Can't generate copy|move constructor with noncopyable mode.\n";
	print $get_options->usage;
	exit 1;
}

my $class = $opts->{classname} or print($get_options->usage), exit 1;
my ($file_title) = map { local $_ = $_; s/^[A-Z]/lc $&/e; s/[A-Z]/'-' . lc $&/eg; $_ } ($class);
my ($include_guard) = map { local $_ = $_; tr/-/_/; uc } ($file_title);
my $namespace = $opts->{namespace} or print($get_options->usage), exit 1;
$include_guard = uc($namespace) . "_${include_guard}_H";
my $summary = $opts->{summary} || "$class クラス";
my @extends = defined $opts->{extends} ? split(/,/, $opts->{extends}) : ();
my @extend_h_files = map { local $_ = $_; s/^[A-Z]/lc $&/e; s/[A-Z]/'-' . lc $&/eg; "$_.h" } @extends;
my ($is_pimpl, $is_interface, $is_virtual, $is_noncopyable, $is_copy_constructor, $is_move_constructor)
	= @$opts{qw(pimpl interface virtual noncopyable copy move)};
$is_virtual = $is_virtual || $is_interface || @extends;

open my $FH, '>', "$file_title.h" or die "Can't open file $file_title.h";
print $FH <<ENDLINE;
/**
	\@file	$file_title.h
	\@brief	${summary}定義
*/
#ifndef $include_guard
#define $include_guard

ENDLINE
print $FH qq(#include "$_"\n) for @extend_h_files;
print $FH "\n" if @extend_h_files;
print $FH "#include <memory>\n" if $is_pimpl;
print $FH "#include <boost/noncopyable.hpp>\n" if $is_noncopyable;
print $FH "\n" if $is_pimpl || $is_noncopyable;
print $FH <<ENDLINE;
namespace $namespace {

/**
	\@brief	$summary
*/
ENDLINE
print $FH "class $class";
print $FH " : " if @extends || $is_noncopyable;
print $FH 'public ' . join(', public ', @extends) if @extends;
print $FH (@extends ? ', ' : '') . "private boost::noncopyable" if $is_noncopyable;
print $FH " {\n";
if ($is_pimpl) {
	print $FH <<ENDLINE
	struct Impl;
	std::unique_ptr<Impl> impl;	///< 実装オブジェクト

ENDLINE
}
print $FH "public:\n";
print $FH <<ENDLINE unless $is_interface;
	/**
		\@brief	デフォルトコンストラクタ
	*/
	$class();
ENDLINE
print $FH <<ENDLINE if $is_copy_constructor;
	/**
		\@brief コピーコンストラクタ
	*/
	$class($class const& src);
	/**
		\@brief コピー代入演算子
	*/
	$class & operator=($class const& src);
ENDLINE
print $FH <<ENDLINE if $is_move_constructor;
	/**
		\@brief ムーブコンストラクタ
	*/
	$class($class && src);
	/**
		\@brief ムーブ代入演算子
	*/
	$class & operator=($class && src);
ENDLINE
print $FH <<ENDLINE;
	/**
		\@brief	デストラクタ
	*/
ENDLINE
print $FH "\t" . ($is_virtual ? 'virtual ' : '') . "~$class()" . ($is_interface ? ' = default' : '') . ";\n";
print $FH <<ENDLINE;
};

}	//namespace $namespace

#endif	//$include_guard
ENDLINE
close $FH;

exit 0 if $is_interface;

open my $FCPP, '>', "$file_title.cpp" or die "Can't open $file_title.cpp";
print $FCPP <<ENDLINE;
/**
	\@file	$file_title.cpp
	\@brief	${summary}実装
*/

#include "$file_title.h"
ENDLINE
print $FCPP <<ENDLINE if $is_move_constructor;

#include <utility>
ENDLINE
print $FCPP <<ENDLINE;

namespace $namespace {

ENDLINE
if ($is_pimpl) {
	print $FCPP <<ENDLINE;
/**
	\@brief	$class クラスの実装オブジェクトクラス
*/
struct ${class}::Impl {
ENDLINE
	print $FCPP <<ENDLINE if $is_copy_constructor;
	/**
		\@brief コピーコンストラクタ
	*/
	Impl(Impl const& src)
	{
		//TODO: コピー生成を行う処理の詳細をここに記述
	}
	/**
		\@brief	コピー代入演算子
	*/
	Impl & operator=(Impl const& src)
	{
		//TODO: コピー代入を行う処理の詳細をここに記述
	}
ENDLINE
	print $FCPP <<ENDLINE;
};

ENDLINE
}
print $FCPP "${class}::$class()";
if (@extends || $is_pimpl) {
	print $FCPP " :\n\t";
	print $FCPP join(', ', map { "$_\{}" } @extends);
	print $FCPP (@extends ? ', ' : '') . "impl{new ${class}::Impl{}}" if $is_pimpl;
}
print $FCPP "\n{}\n";
if ($is_copy_constructor) {
	print $FCPP "\n${class}::$class($class const& src)";
	print $FCPP " : impl{new ${class}::Impl{*src.impl}}" if $is_pimpl;
	print $FCPP "\n";
	print $FCPP <<ENDLINE;
{
	//TODO: コピー生成を行う処理の詳細をここに記述
}

$class & ${class}::operator=($class const& src)
{
ENDLINE
	print $FCPP "\t*this->impl = *src.impl;\n" if $is_pimpl;
	print $FCPP <<ENDLINE;
	//TODO: コピー代入を行う処理の詳細をここに記述
}
ENDLINE
}
if ($is_move_constructor) {
	print $FCPP "\n${class}::$class($class && src)";
	print $FCPP " : impl{std::move(src.impl)}" if $is_pimpl;
	print $FCPP "\n";
	print $FCPP <<ENDLINE;
{
	//TODO: ムーブ生成を行う処理の詳細をここに記述
}

$class & ${class}::operator=($class && src)
{
ENDLINE
	print $FCPP "\tthis->impl = std::move(src.impl);\n" if $is_pimpl;
	print $FCPP <<ENDLINE;
	//TODO: ムーブ代入を行う処理の詳細をここに記述
}
ENDLINE
}
print $FCPP <<ENDLINE;

${class}::~$class() = default;

}	//namespace $namespace
ENDLINE
close $FCPP;

exit 0;
__END__
