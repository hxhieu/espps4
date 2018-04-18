<template>
  <div class="firmwares">
    <h1>{{ id }}</h1>
    <div class="clearfix">
      <a v-for="p in availablePayloads" :key="p.key" class="button pull-left" :href="`${$config.host()}entry/${p.key}`">{{ p.name }}</a>
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';
export default {
  name: 'firmwares',
  props: {
    id: String,
  },
  computed: {
    ...mapState(['payloads']),
    currentFirmware() {
      return this.id;
    },
    availablePayloads() {
      return this.payloads.filter(f => f.fw === this.id);
    },
  },
};
</script>
