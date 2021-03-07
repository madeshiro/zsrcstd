#ifndef ZSR_CONTAINER_H
#define ZSR_CONTAINER_H
#include "zsr_std.h"

/**
 * @brief A standard generic list
 */
typedef void* zsr_list;
#define z_list(type) zsr_list

/**
 * @brief A standard chain list
 */
typedef void* zsr_chainlist;
typedef void* zsr_chainobj;
#define z_chlist(type) zsr_chainlist

#ifdef __cplusplus
extern "C" {
#endif // c++

// @{ zsr_list functions }@ //

/**
 * @brief Creates a new list.
 * The list will be able to contain only data who's sizeof equals those specified in parameter
 * @param _sizeof (zuint32) - data's sizeof
 * @return A new zsr_list
 */
zsr_list zsrcall zCreateList(zuint32 _sizeof);
/**
 * @brief Deletes zsr_list
 * @param _l (zsr_list) - The list to remove
 * @return A boolean, ztrue on success, zfalse otherwise.
 */
zbool zsrcall zDestroyList(zsr_list _l);

/**
 * @brief Gets list's length
 * @param _l (zsr_list) - List to get the length of
 * @return An unsigned int, the list's length
 */
zuint32 zsrcall zListLength(zsr_list _l);
/**
 * @brief Gets the data's sizeof (sizeof(obj))
 * @param _l (zsr_list) - Concerned list
 * @return An unsigned int, the data's sizeof
 */
zuint32 zsrcall zListObjSizeof(zsr_list _l);

zptr zsrcall __zListGet(zsr_list __l, zuint32 __index);
/**
 * Sets the value of a data from the list at the specified index
 * @param _l (zsr_list) - Concerned list
 * @param _obj (void*) - The value buffer
 * @param _i (zuint32) - Index of the value to modify
 */
void zsrcall zListSet(zsr_list _l, void* _obj, zuint32 _i);

/**
 * @brief Gets a value from the list.
 * @param type - The value's type
 * @param zList - zsr_list
 * @param index - The index to look at
 * @return A value from the list at the specified index.
 */
#define zListGet(type, zList, index) ((type) *((type*) __zListGet(zList, index)))

/**
 * @brief Append an element to the list
 *
 * @param [in] _l (zsr_list) - List to modify
 * @param [in] _obj (void*) - Element to append in the list
 */
void zsrcall zListAppend(zsr_list* _l, void* _obj);
/**
 * @brief Prepend an element to the list
 *
 * @param [in] _l (zsr_list) - List to modify
 * @param [in] _obj (void*) - Element to prepend in the list
 */
void zsrcall zListPrepend(zsr_list* _l, void* _obj);

/**
 * @brief Remove an element from the list
 *
 * @param [in] _l (zsr_list*) - List to modify
 * @param [in] _i (zuint32) - Index
 * @return A boolean, true (int: 0x1) if the element had been successfuly removed, false (int: 0x0) otherwise.
 */
zbool zsrcall zListRemove(zsr_list* _l, zuint32 _i);


// @{ zsr_chainlist functions }@ //

/**
 * @brief Creates a new chain list.
 * The chain list will be able to contain only data who's sizeof equals those specified in parameter
 * @param _sizeof (zuint32) - data's sizeof
 * @return A new zsr_chainlist
 */
zsr_chainlist zsrcall zCreateChlist(zint32 _sizeof);
/**
 * Deletes zsr_chainlist
 * @param _l (zsr_list) - The chain list to remove
 * @param _free (bool) - If _free == true, then all zsr_chainobj are removed,
 * however, you'll have to remove all unnecessaries datas by yourself.
 * @return A boolean, ztrue on success, zfalse otherwise.
 */
zbool zsrcall zDestroyChlist(zsr_chainlist _l, zbool _free);

/**
 * @brief Gets list's length
 * @param _l (zsr_chainlist) - Chainlist to get the length of
 * @return An unsigned int, the list's length
 */
zuint32 zsrcall zChlistLength(zsr_chainlist _l);
/**
 * @brief Gets the data's sizeof (sizeof(obj))
 * @param _l (zsr_chainlist) - Concerned list
 * @return An unsigned int, the data's sizeof
 */
zuint32 zsrcall zChlistObjSizeof(zsr_chainlist _l);

/**
 * @brief Gets next object of the chainlist
 * @param _obj (zsr_chainobj or zsr_chainlist) - The chain list or an object from the chain list.
 * @return The next zsr_chainobj or znull if the end has been reached.
 */
zsr_chainobj zsrcall zChlistNext(zsr_chainobj _obj);
/**
 * Gets previous object of the chainlist
 * @param _obj (zsr_chainobj only) - The chain object
 * @return The previous zsr_chainobj or znull if the begginning has been reached
 * @note This function cannot return a zsr_chainlist.
 */
zsr_chainobj zsrcall zChlistPrev(zsr_chainobj _obj);

zptr zsrcall __zChlistValue(zsr_chainobj _obj);
#define zChlistValue(type, _obj) ((type) *((type*) __zChlistValue(_obj)))

/**
 * Gets the zsr_chainlist from an chainlist's object
 * @param _obj (zsr_chainobj) - The object to get the zsr_chainlist of
 * @return A zsr_chainlist
 */
zsr_chainlist zsrcall zChlistFromObj(zsr_chainobj _obj);

/**
 * Push a value in the chainlist
 * @param _l (zsr_chainlist) - The chainlist to modify.
 * @param _obj (zptr) - The object buffer to push in.
 */
void zsrcall zChlistPush(zsr_chainlist _l, zptr _obj);
/**
 * Pop a value from the chainlist
 * @param _l (zsr_chainlist) - The chainlist to modify.
 * @return The object which has been poped from the list or znull if the chainlist was empty.
 */
zsr_chainobj zsrcall zChlistPop(zsr_chainlist _l);

#ifdef __cplusplus
};
#endif // extern "C"

#endif //ZSR_CONTAINER_H
