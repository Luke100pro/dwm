static void defaultgaps(const Arg *arg);
static void getfacts(Monitor *m, int msize, int ssize, float *mf, float *sf, int *mr, int *sr);
static void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc);
static void incrgaps(const Arg *arg);
static void setgaps(int oh, int ov, int ih, int iv);
static void togglegaps(const Arg *arg);

static void monocle(Monitor *m);
static void tile(Monitor *m);
static void fibonacci(Monitor *m, int s);
static void dwindle(Monitor *m);
static void spiral(Monitor *m);



// Gapps
void defaultgaps(const Arg *arg) {
    setgaps(gappoh, gappov, gappih, gappiv);
}

void getfacts(Monitor *m, int msize, int ssize, float *mf, float *sf, int *mr, int *sr) {
    unsigned int n;
    float mfacts, sfacts;
    int mtotal = 0, stotal = 0;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    mfacts = MIN(n, m->nmaster);
    sfacts = n - m->nmaster;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++)
        if (n < m->nmaster)
          mtotal += msize / mfacts;
        else
          stotal += ssize / sfacts;

    *mf = mfacts;         // total factor of master area
    *sf = sfacts;         // total factor of stack area
    *mr = msize - mtotal; // the remainder (rest) of pixels after an even master split
    *sr = ssize - stotal; // the remainder (rest) of pixels after an even stack split
}

void getgaps(Monitor *m, int *oh, int *ov, int *ih, int *iv, unsigned int *nc) {
  unsigned int n, oe, ie;
  oe = ie = enablegaps;
  Client *c;

  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
  if (smartgaps && n == 1) {
    oe = 0; // outer gaps disabled when only one client
  }

  *oh = m->gappoh*oe; // outer horizontal gap
  *ov = m->gappov*oe; // outer vertical gap
  *ih = m->gappih*ie; // inner horizontal gap
  *iv = m->gappiv*ie; // inner vertical gap
  *nc = n;            // number of clients
}

void incrgaps(const Arg *arg) {
    setgaps(
        selmon->gappoh + arg->i,
        selmon->gappov + arg->i,
        selmon->gappih + arg->i,
        selmon->gappiv + arg->i
    );
}

void setgaps(int oh, int ov, int ih, int iv) {
    if (oh < 0) oh = 0;
    if (ov < 0) ov = 0;
    if (ih < 0) ih = 0;
    if (iv < 0) iv = 0;

    selmon->gappoh = oh;
    selmon->gappov = ov;
    selmon->gappih = ih;
    selmon->gappiv = iv;
    arrange(selmon);
}

void togglegaps(const Arg *arg) {
    enablegaps = !enablegaps;
    arrange(selmon);
}



// Vanilla layots
void tile(Monitor *m) {
    unsigned int i, n, h, r, oe = enablegaps, ie = enablegaps, mw, my, ty;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    if (smartgaps == n) {
        oe = 0; //outer gaps disabled
    }

    if (n > m->nmaster)
        mw = m->nmaster ? (m->ww + m->gappiv*ie) * m->mfact : 0;
    else
        mw = m->ww - 2*m->gappov*oe + m->gappiv*ie;
    for (i = 0, my = ty = m->gappoh*oe, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
        if (i < m->nmaster) {
            r = MIN(n, m->nmaster) - i;
            h = (m->wh - my - m->gappoh*oe - m->gappih*ie * (r - 1)) / r;
            resize(c, m->wx + m->gappov*oe, m->wy + my, mw - (2*c->bw) - m->gappiv*ie, h - (2*c->bw), 0);
            if (my + HEIGHT(c) + m->gappih*ie < m->wh)
            my += HEIGHT(c) + m->gappih*ie;
        } else {
            r = n - i;
            h = (m->wh - ty - m->gappoh*oe - m->gappih*ie * (r - 1)) / r;
            resize(c, m->wx + mw + m->gappov*oe, m->wy + ty, m->ww - mw - (2*c->bw) - 2*m->gappov*oe, h - (2*c->bw), 0);
            if (ty + HEIGHT(c) + m->gappih*ie < m->wh)
                ty += HEIGHT(c) + m->gappih*ie;
        }
}
//void
//tile(Monitor *m)
//{
//  unsigned int i, n, h, mw, my, ty;
//  Client *c;
//
//  for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
//  if (n == 0)
//    return;
//
//  if (n > m->nmaster)
//    mw = m->nmaster ? m->ww * m->mfact : 0;
//  else
//    mw = m->ww;
//  for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
//    if (i < m->nmaster) {
//      h = (m->wh - my) / (MIN(n, m->nmaster) - i);
//      resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), 0);
//      if (my + HEIGHT(c) < m->wh)
//        my += HEIGHT(c);
//    } else {
//      h = (m->wh - ty) / (n - i);
//      resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), 0);
//      if (ty + HEIGHT(c) < m->wh)
//        ty += HEIGHT(c);
//    }
//}

void monocle(Monitor *m) {
    unsigned int n = 0;
    int nx, ny, nw, nh;
    int oh, ov, ih, iv;
    Client *c;

    getgaps(m, &oh, &ov, &ih, &iv, &n);

    nx = m->wx + ov;
    ny = m->wy + oh;
    nw = m->ww - 2*ov;
    nh = m->wh - 2*oh;

    for (c = m->clients; c; c = c->next)
        if (ISVISIBLE(c))
            n++;

    for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
        resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), 0);
}



// Fibonacci layouts
void fibonacci(Monitor *m, int s) {
    unsigned int i, n;
    int nx, ny, nw, nh;
    int oh, ov, ih, iv;
    Client *c;

    getgaps(m, &oh, &ov, &ih, &iv, &n);

    for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if(n == 0)
        return;

    nx = m->wx + ov;
    ny = oh;
    nw = m->ww - 2*ov;
    nh = m->wh - 2*oh;

    for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
        if ((i % 2 && nh / 2 > 2*c->bw) || (!(i % 2) && nw / 2 > 2*c->bw)) {
            if (i < n - 1) {
                if (i % 2)
                    nh = (nh - ih) / 2;
                else
                    nw = (nw - iv) / 2;
                if ((i % 4) == 2 && !s)
                    nx += nw + iv;
                else if ((i % 4) == 3 && !s)
                    ny += nh + ih;
            }

            if ((i % 4) == 0) {
                if (s)
                    ny += nh + ih;
                else
                    ny -= nh + ih;
            }
            else if ((i % 4) == 1)
                nx += nw + iv;
            else if ((i % 4) == 2)
                ny += nh + ih;
            else if ((i % 4) == 3) {
                if (s)
                    nx += nw + iv;
                else
                    nx -= nw + iv;
            }

            if (i == 0) {
                if (n != 1)
                    nw = (m->ww - 2*ov - iv) * m->mfact;
                ny = m->wy + oh;
            }
            else if (i == 1)
                nw = m->ww - nw - iv - 2*ov;

            i++;
        }
        resize(c, nx, ny, nw - (2*c->bw), nh - (2*c->bw), False);
    }
}

void dwindle(Monitor *m) {
    fibonacci(m, 1);
}

void spiral(Monitor *m) {
    fibonacci(m, 0);
}
