#include "imlx.h"

#include <stdlib.h>
#include <stdbool.h>

/*!	\fn t_img *add_img(t_session *s, uint32_t w, uint32_t h, int id)
 *	\brief Adds a new image to the session and updates internals.
 * 
 * 	\param s A pointer to the session the new image is supposed to be created
 * 	for.
 * 	\param w The width of the image.
 * 	\param h The height of the image.
 * 	\param id The id for the image.
 *
 * 	Checks if the session already has a image with the id \a id with
 * 	get_img(). If a image with that id already exists returns NULL and does
 * 	nothing.
 *	Allocates space for a new t_img struct and an array of t_img structs as long
 *	as the number of images associated with the session plus one to have room
 *	for the new image. It also duplicates the title string 
 *	If any of these allocations fail it will return NULL after freeing the
 *	already allocated memory.
 *	After the allocation it calles mlx_new_image() and saves the pointer in the
 *	new t_img struct. If the call to mlx_new_image() fails it returns NULL
 *	after freeing the already allocated memory().
 *	After the succesfull call to mlx_new_image() it finishes initialization on
 *	the image struct, adds the already existing images to the new array as
 *	well as the new image, frees the old array and saves the new array in it's
 *	place.
 *
 * 	\returns the pointer to the new image on success, and NULL on failure.
 */
t_img	*add_img(t_session *s, uint32_t w, uint32_t h, int id) {
	t_img		*tmp;
	t_img		**tmparr;
	uint32_t	i;

	if (get_img(s, id))
		return (NULL);
	tmp = malloc(sizeof(*tmp));
	tmparr = malloc((s->numimg + 1) * sizeof(*tmparr));
	if (tmp == NULL || tmparr == NULL)
		return (free(tmp), free(tmparr), NULL);
	tmp->ptr = mlx_new_image(s->cid, w, h);
	if (tmp->ptr == NULL)
		return (free(tmp), free(tmparr), NULL);
	tmp->id = id;
	tmp->addr = mlx_get_data_addr(tmp->ptr, &tmp->bpp, &tmp->ls, &tmp->en);
	tmp->w = w;
	tmp->h = h;
	tmp->s = s;
	i = 0;
	while (i < s->numimg)
	{
		tmparr[i] = s->img[i];
		++i;
	}
	tmparr[i] = tmp;
	++s->numimg;
	free(s->img);
	s->img = tmparr;
	return (tmp);
}

/*!	\fn t_img *get_img(const t_session *s, int id)
 * 	\brief Checks the session pointed to by \a s for a image with the id \a id.
 *
 * 	\param s A pointer to the t_session struct to be searched.
 * 	\param id The id of the image to look for.
 *
 *	Iterates over the images inside the session and returns the one with a
 *	matching id. If none is found it returns a NULL pointer.
 *
 *	\return A pointer to the t_img struct with a matching id if one could be
 *	found or NULL if no matching t_img struct could be found.
 */ 
t_img	*get_img(const t_session *s, int id) {
	uint32_t	i;

	i = 0;
	while (i < s->numimg)
	{
		if (id == s->img[i]->id)
			return (s->img[i]);
		++i;
	}
	return (NULL);
}

/*!	\fn bool put_img(t_img *i, t_win *w, int x, int y)
 * 	\brief Puts an image to a window.
 *
 * 	\param i A pointer to the t_img struct that should be put to the window.
 * 	\param w A pointer to the t_win struct that the image should be put onto.
 * 	\param x The x offset the image should be put to.
 * 	\param y The y offset the image should be put to.
 *
 *	Wrapped call to mlx_put_image_to_window().
 *	The offsett assumes x = 0 and y = 0 to be the top right corner.
 *
 * 	\return At the moment always returns true
 */
bool	put_img(t_img *i, t_win *w, uint32_t x, uint32_t y) {
	mlx_put_image_to_window(i->s->cid, w->ptr, i->ptr, x, y);
	return (true);
}

/*!	\fn bool img_clear(t_img *i)
 * 	\brief Clears a image (set's it to black)
 *
 * 	\param i A pointer ot the t_img struct that should be cleared.
 *
 *	Iterates over the iamge and set's all it's pixels to 0x00000000 (black).
 *
 * 	\return At the moment always returns true
 */
bool	img_clear(t_img *i) {
	uint32_t	j, k;

	j = 0;
	while (j < i->h) {
		k = 0;
		while (k < i->w) {
			put_pixel(i, j, k, 0x00000000);
			++k;
		}
		++j;
	}
	return (true);
}

/*!	\fn bool img_dest(t_img *i)
 * 	\brief Destroys a image and updates internals.
 *
 * 	\param i A pointer ot the t_img struct that should be destroyed.
 *
 *	Moves the images stored in the image array of the associated session
 *	together, skipping the one pointed to by w. If the pointer \a w is not
 *	present in the associated session returns false.
 *	If the image is present it decreases the \a numwin variable for the
 *	associated session and calls mlx_destroy_image() on the image, frees the
 *	title string and the t_img struct itself.
 *
 * 	\return True on success, false on failure.
 */
bool	img_dest(t_img *i) {
	uint32_t	j;

	j = 0;
	while (j < i->s->numimg && i->s->img[j] != i)
		++j;
	if (j == i->s->numimg)
		return (false);
	while (j + 1 < i->s->numimg)
	{
		i->s->img[j] = i->s->img[j + 1];
		++j;
	}
	i->s->img[j] = NULL;
	--i->s->numimg;
	mlx_destroy_image(i->s->cid, i->ptr);
	free(i);
	return (true);
}
