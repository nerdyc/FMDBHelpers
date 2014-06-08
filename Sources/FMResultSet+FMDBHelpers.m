#import "FMResultSet+FMDBHelpers.h"

@implementation FMResultSet (FMDBHelpers)

- (NSArray *)allRecords
{
  NSMutableArray * allRecords = [[NSMutableArray alloc] init];
  while ([self next])
  {
    [allRecords addObject:self.resultDictionary];
  }
  return allRecords;
}

@end
