/**
 *    Copyright (C) 2013-2014 MongoDB Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include "mongo/base/string_data.h"
#include "mongo/db/record_id.h"

namespace mongo {

class BSONObj;
class Collection;
class IndexDescriptor;
class OperationContext;
class PlanExecutor;

/**
 * The internal planner is a one-stop shop for "off-the-shelf" plans.  Most internal procedures
 * that do not require advanced queries could be served by plans already in here.
 */
class InternalPlanner {
public:
    enum Direction {
        FORWARD = 1,
        BACKWARD = -1,
    };

    enum IndexScanOptions {
        // The client is interested in the default outputs of an index scan: BSONObj of the key,
        // RecordId of the record that's indexed.  The client does its own fetching if required.
        IXSCAN_DEFAULT = 0,

        // The client wants the fetched object and the RecordId that refers to it.  Delegating
        // the fetch to the runner allows fetching outside of a lock.
        IXSCAN_FETCH = 1,
    };

    /**
     * Return a collection scan.  Caller owns pointer.
     */
    static PlanExecutor* collectionScan(OperationContext* txn,
                                        StringData ns,
                                        Collection* collection,
                                        const Direction direction = FORWARD,
                                        const RecordId startLoc = RecordId());

    /**
     * Return an index scan.  Caller owns returned pointer.
     */
    static PlanExecutor* indexScan(OperationContext* txn,
                                   const Collection* collection,
                                   const IndexDescriptor* descriptor,
                                   const BSONObj& startKey,
                                   const BSONObj& endKey,
                                   bool endKeyInclusive,
                                   Direction direction = FORWARD,
                                   int options = 0);
};

}  // namespace mongo
