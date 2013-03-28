#include "index.h"

static void _index_recursive_write(FILE * output, ShardNode * node,
                                   unsigned char * data, int depth,
                                   int entrySize, int writeSize);

IndexType index_type_from_string(const char * str) {
    if (strcmp(str, "edgefront") == 0) {
        return IndexTypeEdgeFront;
    } else if (strcmp(str, "edgeback") == 0) {
        return IndexTypeEdgeBack;
    } else if (strcmp(str, "edgeall") == 0) {
        return IndexTypeEdgeAll;
    } else if (strcmp(str, "corners") == 0) {
        return IndexTypeCorners;
    } else return IndexTypeUnknown;
}

int index_type_data_size(IndexType it) {
    if (it == IndexTypeEdgeAll) return 12;
    if (it == IndexTypeCorners) return 8;
    return 6;
}

void index_type_copy_data(IndexType it, unsigned char * data, RubiksMap * map) {
    if (it == IndexTypeCorners) {
        memcpy(data, map->pieces, 8);
    } else if (it == IndexTypeEdgeFront) {
        memcpy(data, &map->pieces[8], 6);
    } else if (it == IndexTypeEdgeBack) {
        memcpy(data, &map->pieces[14], 6);
    } else if (it == IndexTypeEdgeAll) {
        memcpy(data, &map->pieces[8], 6);
    }
}

void index_file_write(IndexType it,
                      int writeSize,
                      int dataSize,
                      int maxDepth,
                      ShardNode * baseNode,
                      FILE * output) {
    unsigned char theMoveMax = maxDepth;
    unsigned char type = it;
    fprintf(output, "ANC2");
    fwrite(&type, 1, 1, output);
    fwrite(&theMoveMax, 1, 1, output);
    char dataBuffer[64];
    _index_recursive_write(output, baseNode,
                           dataBuffer, 0, dataSize,
                           writeSize);
}

ShardNode * index_file_read(const char * path,
                            IndexType * t,
                            unsigned char * moveMax) {
    // read the file header
    FILE * fp = fopen(path, "r");
    if (!fp) return NULL;
    char buffer[4];
    if (fread(buffer, 1, 4, fp) != 4) goto failure;
    if (memcmp(buffer, "ANC2", 4) != 0) goto failure;
    unsigned char typeChar = 0;
    if (fread(&typeChar, 1, 1, fp) != 1) goto failure;
    if (typeChar < 1 || typeChar > 4) goto failure; // not a known type
    *t = typeChar;
    if (fread(moveMax, 1, 1, fp) != 1) goto failure;
    
    // calculate the data size and entry count
    int dataSize = index_type_data_size(*t) + 1;
    fseek(fp, 0, SEEK_END);
    long long offset = ftello(fp);
    fseek(fp, 6, SEEK_SET);
    int entryCount = (offset - 6) / dataSize;
    // make sure that the file isn't miss-aligned
    if (entryCount * dataSize + 6 != offset) goto failure;
    
    ShardNode * node = (ShardNode *)malloc(sizeof(ShardNode));
    bzero(node, sizeof(ShardNode));
    node->depthRemaining = 0;
    node->nodeData = (unsigned char *)malloc(offset - 6);
    node->nodeDataAlloc = entryCount;
    node->nodeDataSize = entryCount;
    if (fread(node->nodeData, 1, offset - 6, fp) != offset - 6) {
        shard_node_free(node);
        goto failure;
    }
    return node;
    
failure:
    fclose(fp);
    return NULL;
}

static void _index_recursive_write(FILE * output, ShardNode * node,
                                   unsigned char * data, int depth,
                                   int entrySize, int writeSize) {
    if (depth > 0) {
        data[depth - 1] = node->nodeCharacter;
    }
    int i;
    if (node->depthRemaining == 0) {
        // just write the data, straightup
        for (i = 0; i < node->nodeDataSize; i++) {
            unsigned char * restData = &node->nodeData[(entrySize - depth) * i];
            memcpy(&data[depth], restData, writeSize - depth);
            fwrite(data, 1, writeSize, output);
        }
        return;
    }
    for (i = 0; i < node->subnodeCount; i++) {
        _index_recursive_write(output, node->subnodes[i], data,
                               depth + 1, entrySize, writeSize);
    }
}
