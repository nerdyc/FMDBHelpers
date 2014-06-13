#define EXP_SHORTHAND
#define PENDING nil

#import <Specta/Specta.h>
#import <Expecta/Expecta.h>
#import "FMDatabase+FMDBHelpers.h"
#import "FMResultSet+FMDBHelpers.h"
#import "FMDatabase+FMDBSpecHelpers.h"
#import <FMDB/FMDatabaseAdditions.h>

SpecBegin(FMDatabase_FMDBHelpers)

__block FMDatabase * database;
__block NSError * error;
beforeEach(^{
  database = [FMDatabase databaseWithPath:nil];
  if (NO == [database open])
  {
    [NSException raise:NSInternalInconsistencyException
                format:@"Error creating database: %@", database.lastError];
  }
});

afterEach(^{
  database = nil;
  error = nil;
});

// ========== TABLES ===================================================================================================
#pragma mark - Tables

describe(@"- createTableWithName:columns:constraints:error:", ^{
  
  it(@"creates a table with the given name and columns", ^{
    [database createTableWithName:@"people"
                          columns:@[ @"id INTEGER PRIMARY KEY", @"firstName TEXT", @"lastName TEXT" ]
                      constraints:nil];
    
    expect([database tableNames]).to.equal([NSSet setWithObject:@"people"]);
    
    expect([database columnExists:@"id" inTableWithName:@"people"]).to.beTruthy();
    expect([database columnExists:@"firstName" inTableWithName:@"people"]).to.beTruthy();
    expect([database columnExists:@"lastName" inTableWithName:@"people"]).to.beTruthy();
  });
  
  it(@"provides an error if the table cannot be created", ^{
    BOOL result = [database createTableWithName:@"people"
                                        columns:nil // at least one column required
                                    constraints:nil
                                          error:&error];
    expect(result).to.beFalsy();
    expect(error).notTo.beNil();
  });
  
});

describe(@"- renameTable:to:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });
  
  it(@"renames the table", ^{
    [database renameTable:@"people"
                       to:@"persons"];
    
    expect([database tableNames]).to.equal([NSSet setWithObject:@"persons"]);
  });
  
  it(@"provides an error if the operation fails", ^{
    BOOL result = [database renameTable:@"xyz"
                                     to:@"persons"
                                  error:&error];
    
    expect(result).to.beFalsy();
    expect(error).notTo.beNil();
  });
  
});

describe(@"- addColumn:toTable:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });
  
  it(@"adds the column to the table", ^{
    [database addColumn:@"middleName TEXT NOT NULL DEFAULT ''"
                toTable:@"people"];
    
    expect([database columnExists:@"middleName"
                  inTableWithName:@"people"]).to.beTruthy();
  });
  
  it(@"provides an error if the operation fails", ^{
    BOOL result = [database addColumn:@"sdfsdf"
                              toTable:@"not_a_table"
                                error:&error];
    
    expect(result).to.beFalsy();
    expect(error).notTo.beNil();
  });
  
});

describe(@"- dropTableWithName:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    [database createTableWithName:@"emailAddresses"
                          columns:@[ @"personId", @"emailAddress" ]];
  });
  
  it(@"removes the table from the database", ^{
    [database dropTableWithName:@"emailAddresses"];
    
    expect([database tableNames]).to.equal([NSSet setWithObject:@"people"]);
  });
  
  it(@"provides an error if the operation fails", ^{
    BOOL result = [database dropTableWithName:@"not_a_table"
                                        error:&error];
    
    expect(result).to.beFalsy();
    expect(error).notTo.beNil();
  });
  
});

describe(@"- dropTableIfExistsWithName:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    [database createTableWithName:@"emailAddresses"
                          columns:@[ @"personId", @"emailAddress" ]];
  });
  
  it(@"removes the table from the database if it exists", ^{
    [database dropTableIfExistsWithName:@"emailAddresses"];
    
    expect([database tableNames]).to.equal([NSSet setWithObject:@"people"]);
  });
  
  it(@"doesn't provide an error if the table doesn't exist", ^{
    BOOL result = [database dropTableIfExistsWithName:@"not_a_table"
                                                error:&error];
    
    expect(result).to.beTruthy();
    expect(error).to.beNil();
  });
  
});

// ========== INDEXES ==================================================================================================
#pragma mark - Indexes

describe(@"- createIndexNamed:onTableNamed:columns:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });
  
  it(@"creates an index with the given columns", ^{
    [database createIndexWithName:@"name_index"
                        tableName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    expect([database indexNamesOnTable:@"people"]).to.equal([NSSet setWithObject:@"name_index"]);
  });
  
});

describe(@"- dropIndexNamed:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    [database createIndexWithName:@"name_index"
                        tableName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });

  it(@"removes the index", ^{
    [database dropIndexWithName:@"name_index"];
    
    expect([database indexNamesOnTable:@"people"]).to.equal([NSSet set]);
  });
  
});

// ========== INSERT ===================================================================================================
#pragma mark - Insert

describe(@"- insertInto:columns:values:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });
  
  it(@"inserts the values into the table", ^{
    [database insertInto:@"people"
                 columns:@[ @"firstName", @"lastName" ]
                  values:@[ @[ @"Amelia", @"Ferris" ],
                            @[ @"Earl",   @"Grey" ] ]];
     
    expect([database countFrom:@"people"]).to.equal(2);
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"Amelia",
                                                                 @"lastName":  @"Ferris" },
                                                              
                                                              @{ @"firstName": @"Earl",
                                                                 @"lastName":  @"Grey" } ]);
  });
  
});

describe(@"- insertInto:dictionaries:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
  });
  
  it(@"inserts the values into the table", ^{
    [database insertInto:@"people"
            dictionaries:@[ @{ @"firstName": @"Amelia" },
                            @{ @"firstName": @"Earl",
                               @"lastName" : @"Grey" } ]];
    
    expect([database countFrom:@"people"]).to.equal(2);
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"Amelia",
                                                                 @"lastName" : [NSNull null] },
                                                              @{ @"firstName": @"Earl",
                                                                 @"lastName":  @"Grey" } ]);
  });
  
});

// ========== UPDATE ===================================================================================================
#pragma mark - Update

describe(@"- update:values:where:arguments:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    [database insertInto:@"people"
                 columns:@[ @"firstName", @"lastName" ]
                  values:@[ @[ @"Amelia", @"Ferris" ],
                            @[ @"Earl",   @"Grey" ] ]];
  });
  
  it(@"updates the values that match the where clause", ^{
    NSInteger numberUpdated = [database update:@"people"
                                     values:@{ @"lastName": @"Gris" }
                                         where:@"lastName = ?"
                                     arguments:@[ @"Ferris" ]];
    
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"Amelia",
                                                                 @"lastName":  @"Gris" },
                                                              
                                                              @{ @"firstName": @"Earl",
                                                                 @"lastName":  @"Grey" } ]);
    expect(numberUpdated).to.equal(1);
  });

  it(@"updates all values when the where clause is missing", ^{
    NSInteger numberUpdated = [database update:@"people"
                                     values:@{ @"lastName": @"Gris" }
                                         where:nil
                                     arguments:nil];
    
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"Amelia",
                                                                 @"lastName":  @"Gris" },
                                                              
                                                              @{ @"firstName": @"Earl",
                                                                 @"lastName":  @"Gris" } ]);
    expect(numberUpdated).to.equal(2);
  });

  
  it(@"returns 0 when no results match", ^{
    NSInteger numberUpdated = [database update:@"people"
                                     values:@{ @"lastName": @"Gris" }
                                         where:@"lastName = ?"
                                     arguments:@[ @"Gladstone" ]];
    expect(numberUpdated).to.equal(0);
  });
  
  it(@"returns -1 when there is an error", ^{
    NSError * error = nil;
    NSInteger numberUpdated = [database update:@"people"
                                     values:@{ @"lastName": @"Gris" }
                                         where:@"age = ?" // age is an unknown column
                                     arguments:@[ @21 ]
                                         error:&error];
    
    expect(numberUpdated).to.beLessThan(0);
    expect(error).notTo.beNil();
  });
  
});

describe(@"- update:columns:expressions:where:arguments:error:", ^{

  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    [database insertInto:@"people"
                 columns:@[ @"firstName", @"lastName" ]
                  values:@[ @[ @"Amelia", @"Ferris" ],
                            @[ @"Earl",   @"Grey" ] ]];
  });
  
  it(@"updates values with the given expressions", ^{
    NSInteger numberUpdated = [database update:@"people"
                                       columns:@[ @"firstName",         @"lastName" ]
                                   expressions:@[ @"lower(firstName)",  @"upper(lastName)" ]
                                         where:nil
                                     arguments:nil];
    
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"amelia",
                                                                 @"lastName":  @"FERRIS" },
                                                              
                                                              @{ @"firstName": @"earl",
                                                                 @"lastName":  @"GREY" } ]);
    expect(numberUpdated).to.equal(2);

  });
  
});

// ========== DELETE ===================================================================================================
#pragma mark - Delete

describe(@"- deleteFrom:where:arguments:error:", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    [database insertInto:@"people"
                 columns:@[ @"firstName", @"lastName" ]
                  values:@[ @[ @"Amelia", @"Ferris" ],
                            @[ @"Earl",   @"Grey" ] ]];
  });
  
  it(@"deletes the values that match the where clause", ^{
    NSInteger numberDeleted = [database deleteFrom:@"people"
                                             where:@"lastName = ?"
                                         arguments:@[ @"Ferris" ]];
    
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[ @{ @"firstName": @"Earl",
                                                                 @"lastName":  @"Grey" } ]);
    expect(numberDeleted).to.equal(1);
  });
  
  it(@"deletes all values when the where clause is missing", ^{
    NSInteger numberDeleted = [database deleteFrom:@"people"
                                             where:nil
                                         arguments:nil];
    
    expect([database selectAllFrom:@"people"
                           orderBy:@"firstName"]).to.equal(@[]);
    expect(numberDeleted).to.equal(2);
  });
  
  
  it(@"returns 0 when no results match", ^{
    NSInteger numberDeleted = [database deleteFrom:@"people"
                                             where:@"lastName = ?"
                                         arguments:@[ @"Gladstone" ]];
    expect(numberDeleted).to.equal(0);
  });
  
  it(@"returns -1 when there is an error", ^{
    NSError * error = nil;
    NSInteger numberDeleted = [database deleteFrom:@"people"
                                             where:@"age = ?" // age is an unknown column
                                         arguments:@[ @21 ]
                                             error:&error];
    
    expect(numberDeleted).to.beLessThan(0);
    expect(error).notTo.beNil();
  });
  
});

// ========== SELECT ===================================================================================================
#pragma mark - Select

describe(@"- selectResultsFrom:matchingValues:orderBy:error", ^{
  
  beforeEach(^{
    [database createTableWithName:@"people"
                          columns:@[ @"firstName", @"lastName" ]];
    
    [database insertInto:@"people"
                 columns:@[ @"firstName", @"lastName" ]
                  values:@[ @[ @"Amelia", @"Grey" ],
                            @[ @"Earl",   @"Grey" ],
                            @[ @"Earl",   @"Green" ],
                            @[ @"James",  @"Green" ] ]];
  });
  
  it(@"selects rows that match the values", ^{
    NSArray * results = [database selectResultsFrom:@"people"
                                     matchingValues:@{ @"lastName": @"Grey" }
                                            orderBy:@"firstName"
                                              error:NULL].allRecords;
    
    expect(results).to.equal(@[ @{ @"firstName": @"Amelia", @"lastName": @"Grey" },
                                @{ @"firstName": @"Earl",   @"lastName": @"Grey" } ]);
  });
  
  it(@"selects only rows that match all the values", ^{
    NSArray * results = [database selectResultsFrom:@"people"
                                     matchingValues:@{ @"firstName": @"Earl", @"lastName": @"Grey" }
                                            orderBy:nil
                                              error:NULL].allRecords;
    
    expect(results).to.equal(@[ @{ @"firstName": @"Earl", @"lastName": @"Grey" } ]);
  });
  
  it(@"selects any row that matches a value in an NSArray", ^{
    NSArray * results = [database selectResultsFrom:@"people"
                                     matchingValues:@{ @"firstName": @[ @"Amelia", @"James" ] }
                                            orderBy:nil
                                              error:NULL].allRecords;
    
    expect(results).to.equal(@[ @{ @"firstName": @"Amelia", @"lastName": @"Grey" },
                                @{ @"firstName": @"James",  @"lastName": @"Green" } ]);
  });
  
});

SpecEnd
