import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex);

export const PAYLOAD = 'PAYLOAD';

export default new Vuex.Store({
  state: {
    payloads: [],
  },
  mutations: {
    [PAYLOAD]: (state, payloads) => {
      state.payloads = payloads;
    },
  },
  actions: {
    storePayloads: ({ commit }, payloads) => {
      commit(PAYLOAD, payloads);
    },
  },
});
