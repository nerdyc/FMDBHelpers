#import "FMResultSet.h"

@interface FMResultSet (FMDBHelpers)

/**
 *  Reads all results and returns them as an array of dictionaries.
 */
- (NSArray *)allRecords;

@end
