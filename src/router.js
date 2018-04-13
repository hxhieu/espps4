import Vue from 'vue';
import Router from 'vue-router';
import Home from './views/Home.vue';
import Firmware from './views/Firmware.vue';
import NotFound from './views/NotFound';

Vue.use(Router);

export default new Router({
  routes: [
    {
      path: '/',
      component: Home,
    },
    {
      path: '/firmware/:id',
      props: true,
      component: Firmware,
    },
    {
      path: '*',
      component: NotFound,
    },
  ],
});
