import Vue from 'vue';
import Router from 'vue-router';
import Home from './views/Home.vue';
import Firmware from './views/Firmware.vue';
import NotFound from './views/NotFound';
import Credits from './views/Credits';
import Settings from './views/Settings';
import Payloads from './views/Payloads';

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
      path: '/credits',
      component: Credits,
    },
    {
      path: '/settings',
      component: Settings,
    },
    {
      path: '/payloads/:firm',
      props: true,
      component: Payloads,
    },
    {
      path: '*',
      component: NotFound,
    },
  ],
});
