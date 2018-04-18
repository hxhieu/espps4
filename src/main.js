import Vue from 'vue';
import App from './App.vue';
import router from './router';
import store from './store';

import 'pretty-checkbox/dist/pretty-checkbox.css';

import PrettyCheckbox from 'pretty-checkbox-vue';
import HttpClient from '@/plugins/http-client';
import Configuration from '@/plugins/configuration';

Vue.use(PrettyCheckbox);
Vue.use(HttpClient);
Vue.use(Configuration);

//import 'bulma/css/bulma.css';                       // 160Kb
//import 'vuetify/dist/vuetify.css';                  // 245Kb
//import 'iview/dist/styles/iview.css';               // 220Kb
//import 'at-ui-style';                               // 120Kb
//import 'vue-mdc-adapter/dist/vue-mdc-adapter.css';  // 183Kb
//import 'buefy/lib/buefy.css';                       // 209Kb

Vue.config.productionTip = false;

new Vue({
  router,
  store,
  render: h => h(App),
}).$mount('#app');
