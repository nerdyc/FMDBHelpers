# FMDBHelpers

`FMDBHelpers` provides helper methods for executing common SQL statements with [FMDB](https://github.com/ccgus/FMDB).

For example, instead of:

		FMResultSet * results = [db executeQuery:@"SELECT * FROM people ORDER BY firstName"];
		NSMutableArray * people = [NSMutableArray array];
		while ([results next])
		{
    		[people addObject:results.resultsDictionary];
		}

You can simply write:
		
		NSArray * people = [db selectAllFrom:@"people" orderBy:@"firstName" error:NULL];

In addition to basic SELECT statements, there are also helpers for creating tables and indexes, inserting, updating, and deleting records.

## Usage

FMDBHelpers contains categories for `FMDatabase` and `FMResultSet`. Both are documented on [cocoadocs.org](http://cocoadocs.org/docsets/FMDBHelpers/)

## Installation

`FMDBHelpers` can be installed with Cocoapods:

		pod 'FMDBHelpers'

## Contributors

Contributors are listed in [CONTRIBUTORS.md](CONTRIBUTORS.md). Thanks!

## License

`FMDBHelpers` is covered by the MIT license. See the LICENSE file for full text.