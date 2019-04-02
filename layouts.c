void
grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

static void deck(Monitor *m);

void
deck(Monitor *m) {
  int dn;
  unsigned int i, n, h, mw, my;
  Client *c;

  for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
  if(n == 0)
    return;

  dn = n - m->nmaster;
  if(dn > 0) /* override layout symbol */
    snprintf(m->ltsymbol, sizeof m->ltsymbol, "D %d", dn);

  if(n > m->nmaster)
    mw = m->nmaster ? m->ww * m->mfact : 0;
  else
    mw = m->ww;
  for(i = my = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
    if(i < m->nmaster) {
      h = (m->wh - my) / (MIN(n, m->nmaster) - i);
      resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), False);
      my += HEIGHT(c);
    }
    else
      resize(c, m->wx + mw, m->wy, m->ww - mw - (2*c->bw), m->wh - (2*c->bw), False);
}