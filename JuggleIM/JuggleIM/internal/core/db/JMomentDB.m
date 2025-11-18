//
//  JMomentDB.m
//  JuggleIM
//
//  Created by Fei Li on 2025/11/17.
//

#import "JMomentDB.h"

NSString *const jCreateMomentTable = @"CREATE TABLE IF NOT EXISTS moment ("
                                    "moment_id VARCHAR (64) PRIMARY KEY,"
                                    "content TEXT,"
                                    "media_array TEXT,"
                                    "user_info TEXT,"
                                    "reaction_array TEXT,"
                                    "comment_array TEXT,"
                                    "create_time INTEGER,"
                                    "update_time INTEGER"
                                    ")";
NSString *const jGetMoments = @"SELECT * FROM moment WHERE ";
NSString *const jUpdateMoments = @"INSERT OR REPLACE INTO moment (moment_id, content, media_array, user_info, reaction_array, comment_array, create_time, update_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";


@interface JMomentDB ()
@property (nonatomic, strong) JDBHelper *dbHelper;
@end

@implementation JMomentDB

- (void)createTables {
    [self.dbHelper executeUpdate:jCreateMomentTable withArgumentsInArray:nil];
}

- (NSArray<JMoment *> *)getCachedMomentList:(JGetMomentOption *)option {
    NSString *sql = jGetMoments;
    NSMutableArray *args = [NSMutableArray array];
    
    if (option.direction == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:@" timestamp > ?"];
    } else {
        sql = [sql stringByAppendingString:@" timestamp < ?"];
    }
    [args addObject:@(option.startTime)];
    sql = [sql stringByAppendingString:@" ORDER BY create_time"];
    if (option.direction == JPullDirectionNewer) {
        sql = [sql stringByAppendingString:@" ASC"];
    } else {
        sql = [sql stringByAppendingString:@" DESC"];
    }
    sql = [sql stringByAppendingString:@" LIMIT ?"];
    [args addObject:@(option.count)];
    NSMutableArray *momentList = [NSMutableArray array];
    [self.dbHelper executeQuery:sql
           withArgumentsInArray:args
                     syncResult:^(JFMResultSet * _Nonnull resultSet) {
        while ([resultSet next]) {
            JMoment *moment = [self momentWith:resultSet];
            [momentList addObject:moment];
        }
    }];
    return [momentList copy];
}

- (void)updateMomentList:(NSArray<JMoment *> *)momentList {
    [self.dbHelper executeTransaction:^(JFMDatabase * _Nonnull db, BOOL * _Nonnull rollback) {
        for (JMoment *moment in momentList) {
                NSString *mediaArrayStr = [self jsonStringFromArray:moment.mediaArray];
                NSString *userInfoStr = [self jsonStringFromObject:moment.userInfo];
                NSString *reactionArrayStr = [self jsonStringFromArray:moment.reactionArray];
                NSString *commentArrayStr = [self jsonStringFromArray:moment.commentArray];
                
                NSString *sql = @"INSERT OR REPLACE INTO moment ("
                "moment_id, content, media_array, user_info, reaction_array, comment_array, create_time, update_time"
                ") VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
                
                [db executeUpdate:sql,
                    moment.momentId ?: [NSNull null], // 防止 nil 导致 SQL 错误
                    moment.content ?: [NSNull null],
                    mediaArrayStr ?: [NSNull null],
                    userInfoStr ?: [NSNull null],
                    reactionArrayStr ?: [NSNull null],
                    commentArrayStr ?: [NSNull null],
                    @(moment.createTime),
                    @(moment.updateTime)];
            }
    }];
    
}

- (nonnull instancetype)initWithDBHelper:(nonnull JDBHelper *)dbHelper {
    if (self = [super init]) {
        self.dbHelper = dbHelper;
    }
    return self;
}

+ (nonnull NSString *)createMomentTable {
    return jCreateMomentTable;
}

- (JMoment *)momentWith:(JFMResultSet *)rs {
    JMoment *moment = [JMoment new];
    
    moment.momentId = [rs stringForColumn:@"moment_id"];
    moment.content = [rs stringForColumn:@"content"];
    moment.createTime = [rs longLongIntForColumn:@"create_time"];
    moment.updateTime = [rs longLongIntForColumn:@"update_time"];
    
    NSString *mediaArrayStr = [rs stringForColumn:@"media_array"];
    if (mediaArrayStr.length > 0) {
        NSData *mediaData = [mediaArrayStr dataUsingEncoding:NSUTF8StringEncoding];
        NSArray *mediaDicts = [NSJSONSerialization JSONObjectWithData:mediaData options:0 error:nil];
        if ([mediaDicts isKindOfClass:[NSArray class]]) {
            NSMutableArray *medias = [NSMutableArray array];
            for (NSDictionary *dict in mediaDicts) {
                JMomentMedia *media = [self mediaFromDict:dict];
                if (media) [medias addObject:media];
            }
            moment.mediaArray = medias.copy;
        }
    }
    
    NSString *userInfoStr = [rs stringForColumn:@"user_info"];
    if (userInfoStr.length > 0) {
        NSData *userData = [userInfoStr dataUsingEncoding:NSUTF8StringEncoding];
        NSDictionary *userDict = [NSJSONSerialization JSONObjectWithData:userData options:0 error:nil];
        moment.userInfo = [self userInfoFromDict:userDict];
    }
    
    NSString *reactionArrayStr = [rs stringForColumn:@"reaction_array"];
    if (reactionArrayStr.length > 0) {
        NSData *reactionData = [reactionArrayStr dataUsingEncoding:NSUTF8StringEncoding];
        NSArray *reactionDicts = [NSJSONSerialization JSONObjectWithData:reactionData options:0 error:nil];
        if ([reactionDicts isKindOfClass:[NSArray class]]) {
            NSMutableArray *reactions = [NSMutableArray array];
            for (NSDictionary *dict in reactionDicts) {
                JMomentReaction *reaction = [self reactionFromDict:dict];
                if (reaction) [reactions addObject:reaction];
            }
            moment.reactionArray = reactions.copy;
        }
    }
    
    NSString *commentArrayStr = [rs stringForColumn:@"comment_array"];
    if (commentArrayStr.length > 0) {
        NSData *commentData = [commentArrayStr dataUsingEncoding:NSUTF8StringEncoding];
        NSArray *commentDicts = [NSJSONSerialization JSONObjectWithData:commentData options:0 error:nil];
        if ([commentDicts isKindOfClass:[NSArray class]]) {
            NSMutableArray *comments = [NSMutableArray array];
            for (NSDictionary *dict in commentDicts) {
                JMomentComment *comment = [self commentFromDict:dict];
                if (comment) [comments addObject:comment];
            }
            moment.commentArray = comments.copy;
        }
    }
    
    return moment;
}

- (JMomentMedia *)mediaFromDict:(NSDictionary *)dict {
    if (![dict isKindOfClass:[NSDictionary class]]) return nil;
    
    JMomentMedia *media = [[JMomentMedia alloc] init];
    media.url = dict[@"url"];
    media.type = [dict[@"type"] integerValue];
    return media;
}

- (JUserInfo *)userInfoFromDict:(NSDictionary *)dict {
    if (![dict isKindOfClass:[NSDictionary class]]) return nil;
    
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = dict[@"userId"];
    userInfo.userName = dict[@"userName"];
    userInfo.portrait = dict[@"portrait"];
    userInfo.extraDic = dict[@"extraDic"];
    userInfo.type = [dict[@"type"] integerValue];
    userInfo.updatedTime = [dict[@"updatedTime"] longLongValue];
    return userInfo;
}

- (JMomentReaction *)reactionFromDict:(NSDictionary *)dict {
    if (![dict isKindOfClass:[NSDictionary class]]) return nil;
    
    JMomentReaction *reaction = [[JMomentReaction alloc] init];
    reaction.key = dict[@"key"];
    
    NSArray *userDicts = dict[@"userArray"];
    if ([userDicts isKindOfClass:[NSArray class]]) {
        NSMutableArray *users = [NSMutableArray array];
        for (NSDictionary *userDict in userDicts) {
            JUserInfo *user = [self userInfoFromDict:userDict];
            if (user) [users addObject:user];
        }
        reaction.userArray = users.copy;
    }
    return reaction;
}

- (JMomentComment *)commentFromDict:(NSDictionary *)dict {
    if (![dict isKindOfClass:[NSDictionary class]]) return nil;
    
    JMomentComment *comment = [[JMomentComment alloc] init];
    comment.commentId = dict[@"commentId"];
    comment.momentId = dict[@"momentId"];
    comment.parentCommentId = dict[@"parentCommentId"];
    comment.content = dict[@"content"];
    comment.createTime = [dict[@"createTime"] longLongValue];
    comment.updateTime = [dict[@"updateTime"] longLongValue];
    comment.userInfo = [self userInfoFromDict:dict[@"userInfo"]];
    comment.parentUserInfo = [self userInfoFromDict:dict[@"parentUserInfo"]];
    
    return comment;
}

- (NSString *)jsonStringFromArray:(NSArray *)array {
    if (!array || array.count == 0) return nil;
    
    NSMutableArray *dictArray = [NSMutableArray array];
    for (id obj in array) {
        if ([obj respondsToSelector:@selector(toDictionary)]) {
            NSDictionary *dict = [obj performSelector:@selector(toDictionary)];
            if (dict) [dictArray addObject:dict];
        }
    }
    
    return [self jsonStringFromObject:dictArray];
}

- (NSString *)jsonStringFromObject:(id)object {
    if (!object) return nil;
    
    NSDictionary *dict = nil;
    if ([object respondsToSelector:@selector(toDictionary)]) {
        dict = [object performSelector:@selector(toDictionary)];
    } else if ([object isKindOfClass:[NSDictionary class]]) {
        dict = object; // 如果是字典直接使用
    }
    
    if (!dict) return nil;
    
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:0 error:nil];
    if (!jsonData) return nil;
    
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}
@end
